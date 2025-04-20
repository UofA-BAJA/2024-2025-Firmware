import tkinter as tk
from tkinter import filedialog, messagebox
import sqlite3
from collections import defaultdict
from datetime import datetime
import csv


class WizardApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Data Wizard")
        self.root.geometry("600x600")

        self.db_path = None
        self.conn = None
        self.cursor = None
        self.selected_session = None
        self.data_types_vars = []

        self.init_step_1()

    def clear_window(self):
        for widget in self.root.winfo_children():
            widget.destroy()

    # STEP 1: File Selection
    def init_step_1(self):
        self.clear_window()

        label = tk.Label(self.root, text="Step 1: Select SQLite DB File", font=("Arial", 14))
        label.pack(pady=20)

        select_btn = tk.Button(self.root, text="Select File", command=self.select_file)
        select_btn.pack(pady=10)

    def select_file(self):
        file_path = filedialog.askopenfilename(filetypes=[("Database Files", "*.db")])
        if file_path:
            self.db_path = file_path
            try:
                self.conn = sqlite3.connect(self.db_path)
                self.cursor = self.conn.cursor()
                self.init_step_2()
            except Exception as e:
                messagebox.showerror("Error", f"Failed to open database:\n{e}")
        else:
            messagebox.showinfo("Info", "No file selected.")

    # STEP 2: Session Selection
    def init_step_2(self):
        self.clear_window()

        tk.Label(self.root, text="Step 2: Select Session", font=("Arial", 14)).pack(pady=20)
        tk.Button(self.root, text="Next", command=self.handle_session_selected).pack(pady=10)

        try:
            sessions = self.cursor.execute("SELECT * FROM Session").fetchall()
        except Exception as e:
            messagebox.showerror("Error", f"Failed to load sessions:\n{e}")
            return

        if not sessions:
            messagebox.showwarning("No Sessions", "No sessions found in this database.")
            self.init_step_1()
            return

        self.session_var = tk.StringVar(value="")

        for session in sessions:
            session_str = f"{session[0]}: {session[1:]}"  # session_id: other info
            tk.Radiobutton(self.root, text=session_str, variable=self.session_var, value=str(session[0])).pack(anchor="w")


    def handle_session_selected(self):
        if not self.session_var.get():
            messagebox.showwarning("Selection Needed", "Please select a session to continue.")
            return
        self.selected_session = int(self.session_var.get())
        self.init_step_3()

    # STEP 3: Data Type Selection
    def init_step_3(self):
        self.clear_window()
        tk.Label(self.root, text="Step 3: Select Data Types", font=("Arial", 14)).pack(pady=20)

        try:
            data_types = self.cursor.execute("""
                SELECT DISTINCT DataType.DataTypeID, DataTypeName, DataUnit
                FROM DataType 
                INNER JOIN Data ON Data.SessionID = ? AND Data.DataTypeID = DataType.DataTypeID
            """, [self.selected_session]).fetchall()
        except Exception as e:
            messagebox.showerror("Error", f"Failed to load data types:\n{e}")
            return

        if not data_types:
            messagebox.showwarning("No Data", "No data types found for the selected session.")
            self.init_step_2()
            return

        self.data_types_vars = []
        
        # Special variable to select all
        special_label = "Select All"
        special_var = tk.IntVar()
        tk.Checkbutton(self.root, text=special_label, variable=special_var).pack(anchor="w")
        self.data_types_vars.append((special_label, special_var))


        for dt in data_types:
            var = tk.IntVar()
            label = (dt[0], dt[1], dt[2])
            tk.Checkbutton(self.root, text=label, variable=var).pack(anchor="w")
            self.data_types_vars.append((label, var))




        tk.Button(self.root, text="Generate CSV", command=self.generate_csv).pack(pady=10)


    def generate_csv(self):

        if(len(self.data_types_vars) == 0):
            messagebox.showerror("No Selection", "No data types selected.")
            return

        selected_ids = []
        
        if(self.data_types_vars[0][1].get()):
            selected_ids = [label[0] for label, var in self.data_types_vars if label != 'Select All']

        else:
            selected_ids = [label[0] for label, var in self.data_types_vars if var.get()]
            if not selected_ids:
                messagebox.showerror("No Selection", "No data types selected.")
                return


        # Now that we have the selected data type ID's and the session ID, 
        # we just have to add them to a CSV file =D


        # Potential issue: If there's a ton of data, we might need to chunk this up somehow. For now it works fine though
        
        data_dict = defaultdict(dict)  # {timestamp: {datatype_id: value}}

        for dtype_id in selected_ids:
            rows = self.cursor.execute(
                "SELECT Timestamp, Value FROM Data WHERE SessionID = ? AND DataTypeID = ?",
                (self.selected_session, dtype_id)
            ).fetchall()

            for timestamp, value in rows:
                data_dict[timestamp][dtype_id] = value

        if not data_dict:
            messagebox.showinfo("No Data", "No data found for selected types.")
            return

        all_timestamps = sorted(data_dict.keys())

        # Optional: Map DataTypeID to nicer label
        dtype_id_to_label = {label[0]: f"{label[1]} ({label[2]})" for label, _ in self.data_types_vars if isinstance(label, tuple)}

        filename = filedialog.asksaveasfilename(
            defaultextension=".csv", filetypes=[("CSV Files", "*.csv")], title="Save CSV"
        )
        if not filename:
            return

        try:
            with open(filename, "w", newline="") as f:
                writer = csv.writer(f)
                # Header
                header = ["Timestamp"] + [dtype_id_to_label[dtid] for dtid in selected_ids]
                writer.writerow(header)

                for timestamp in all_timestamps:
                    row = [timestamp]
                    for dtid in selected_ids:
                        row.append(data_dict[timestamp].get(dtid, ""))
                    writer.writerow(row)

            messagebox.showinfo("Success", f"CSV file saved to:\n{filename}")
            self.root.quit()

        except Exception as e:
            messagebox.showerror("Error", f"Failed to save CSV:\n{e}")
        


if __name__ == "__main__":
    root = tk.Tk()
    app = WizardApp(root)
    root.mainloop()
