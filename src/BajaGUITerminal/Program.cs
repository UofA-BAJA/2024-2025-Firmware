// Program.cs  ─────────────────────────────────────────────────────────────
// Terminal.Gui 1.5.0  •  ConsolePlot 0.1.0
//
// Controls
//   Tab / Shift‑Tab         : cycle yellow focus box
//   t                       : change data‑type of focused pane
//   c                       : send command to MCU
//   Esc / q / Q / Ctrl‑Q    : quit
//
// Build & run
//   dotnet add package Terminal.Gui --version 1.5.0
//   dotnet add package ConsolePlot  --version 0.1.0
//   dotnet run
// -------------------------------------------------------------------------

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Terminal.Gui;
using ConsolePlot;
using ConsolePlot.Drawing.Tools;
// alias to disambiguate Attribute type
using TAttribute = Terminal.Gui.Attribute;

namespace Baja.TerminalTelemetry
{
    internal static class Program
    {
        private const int PaneCount = 4;

        private static readonly DataType[] DefaultTypes =
        {
            DataType.RPM_FRONT_L,
            DataType.RPM_FRONT_R,
            DataType.CAR_SPEED,
            DataType.CVT_TEMPERATURE
        };

        private static void Main()
        {
            // ── boot the TUI ────────────────────────────────────────────
            Application.Init();

            // ──────── LIGHT‑THEME COLOR SCHEME ────────────────────────
            var lightScheme = new ColorScheme
            {
                // unselected controls / list items
                Normal    = new TAttribute(Color.Black,  Color.White),

                // control or list‑row that currently has keyboard focus
                Focus     = new TAttribute(Color.Black,  Color.BrightYellow),

                // hotkey characters (underscored letters)
                HotNormal = new TAttribute(Color.BrightBlue, Color.White),
                HotFocus  = new TAttribute(Color.BrightBlue, Color.BrightYellow)
            };

            var top = Application.Top;

            var root = new Window("BAJA – Live Telemetry (TUI)")
            {
                X = 0,
                Y = 0,
                Width  = Dim.Fill(),
                Height = Dim.Fill()
            };

            top.ColorScheme  = lightScheme;   // apply globally
            root.ColorScheme = lightScheme;
            top.Add(root);

            // ── 2 × 2 plot panes ───────────────────────────────────────
            var panes       = new PlotPane[PaneCount];
            var paneForType = new Dictionary<DataType, int>();

            for (int i = 0; i < PaneCount; i++)
            {
                var pane = new PlotPane(DefaultTypes[i])
                {
                    X = (i % 2 == 0) ? 0            : Pos.Percent(50),
                    Y = (i < 2)      ? 0            : Pos.Percent(50),
                    Width  = Dim.Percent(50),
                    Height = Dim.Percent(50)
                };
                pane.ColorScheme = lightScheme;
                root.Add(pane);
                panes[i] = pane;
                paneForType[DefaultTypes[i]] = i;
            }

            // ── data layer ─────────────────────────────────────────────
            var dm = new DataManager("/dev/ttyUSB0", 115_200);
            dm.SampleReady += (dt, t, v) =>
            {
                if (paneForType.TryGetValue(dt, out var idx))
                    panes[idx].Push(t, v);
            };

            // ── global key‑handling ───────────────────────────────────
            int active = 0;
            panes[active].SetActive(true);

            Application.RootKeyEvent += ke =>
            {
                Key k = ke.Key;

                // quit
                if (k is Key.Esc or Key.Q or Key.q or (Key.CtrlMask | Key.Q))
                {
                    Application.RequestStop();
                    return true;
                }

                // cycle focus
                if (k == Key.Tab || k == (Key.Tab | Key.ShiftMask))
                {
                    int dir = (k & Key.ShiftMask) != 0 ? -1 : +1;
                    panes[active].SetActive(false);
                    active = (active + dir + PaneCount) % PaneCount;
                    panes[active].SetActive(true);
                    panes[active].SetFocus();
                    return true;
                }

                // data‑type picker
                if (k == (Key) 't')
                {
                    ShowTypeDialog(panes[active], active);
                    return true;
                }

                // send command
                if (k == (Key) 'c')
                {
                    ShowCommandDialog();
                    return true;
                }

                return false; // not handled
            };

            // refresh plots every 100 ms
            Application.MainLoop.AddTimeout(
                TimeSpan.FromMilliseconds(100),
                _ =>
                {
                    foreach (var p in panes) p.Refresh();
                    return true;
                });

            Application.Run();

            // ───────────────────────── helper dialogs ─────────────────

            void ShowTypeDialog(PlotPane pane, int paneIdx)
            {
                var types = Enum.GetValues<DataType>().ToArray();
                var list  = new ListView(types.Select(t => t.ToString()).ToList())
                            { Width = Dim.Fill(), Height = Dim.Fill() };

                var dlg   = new Dialog("Select DataType", 60, 20, new Button("Cancel", is_default: true))
                { ColorScheme = lightScheme };
                dlg.Add(list);

                list.OpenSelectedItem += args =>
                {
                    var chosen = types[args.Item];
                    pane.ChangeType(chosen);

                    paneForType.RemoveAll(kv => kv.Value == paneIdx);
                    paneForType[chosen] = paneIdx;

                    Application.RequestStop();
                };
                Application.Run(dlg);
            }

            void ShowCommandDialog()
            {
                var cmds = Enum.GetValues<Command>().ToArray();
                var list = new ListView(cmds.Select(c => $"{c} ({(int)c})").ToList())
                           { Width = Dim.Fill(), Height = Dim.Fill() };

                var dlg  = new Dialog("Send Command", 50, 20, new Button("Cancel", is_default: true))
                { ColorScheme = lightScheme };
                dlg.Add(list);

                list.OpenSelectedItem += args =>
                {
                    var chosen = cmds[args.Item];
                    dm.SendCommand(chosen);
                    MessageBox.Query("Command sent",
                                     $"Sent {chosen} (value {(int)chosen})",
                                     "OK");
                    Application.RequestStop();
                };
                Application.Run(dlg);
            }
        }
    }

    // ─────────────────────────── PlotPane ──────────────────────────────
    internal class PlotPane : Window
    {
        private const int W = 60, H = 18, MaxPts = W - 6;

        private readonly TextView view;
        private readonly List<double> xs = new();
        private readonly List<double> ys = new();

        public PlotPane(DataType dt) : base(dt.ToString())
        {
            view = new TextView
            {
                X = 0,
                Y = 0,
                Width  = Dim.Fill(),
                Height = Dim.Fill(),
                ReadOnly  = true,
                WordWrap  = false,
                Multiline = true
            };
            Add(view);

            Border.BorderStyle     = BorderStyle.Single;
            Border.BorderThickness = new Thickness(1);
            Border.BorderBrush     = Color.Gray;
        }

        public void SetActive(bool on) =>
            Border.BorderBrush = on ? Color.BrightYellow : Color.Gray;

        public void Push(float t, float v)
        {
            xs.Add(t);
            ys.Add(v);
            if (xs.Count > MaxPts)
            {
                xs.RemoveAt(0);
                ys.RemoveAt(0);
            }
        }

        public void Refresh()
        {
            if (xs.Count == 0) return;

            var plt = new Plot(W, H);

            // main data series
            plt.AddSeries(xs, ys);

            // extra 0‑line for scale reference
            if (xs.Count > 1)
            {
                var zeros = Enumerable.Repeat(0.0, xs.Count).ToList();
                plt.AddSeries(xs, zeros,
                    new PointPen(SystemPointBrushes.Braille, ConsoleColor.DarkGray));
            }

            plt.Draw();

            var img = plt.GetImage();
            var sb  = new StringBuilder(H * (W + 1));
            for (int y = img.Height - 1; y >= 0; y--)
            {
                for (int x = 0; x < img.Width; x++)
                    sb.Append(img.GetPixel(x, y).Character);
                sb.AppendLine();
            }
            view.Text = sb.ToString();
            view.SetNeedsDisplay();
        }

        public void ChangeType(DataType dt)
        {
            Title = dt.ToString();
            xs.Clear();
            ys.Clear();
        }
    }

    // helper: wipe dictionary entries by predicate
    internal static class DictExt
    {
        public static void RemoveAll<K, V>(this IDictionary<K, V> d,
                                           Func<KeyValuePair<K, V>, bool> pred)
        {
            foreach (var k in d.Where(pred).Select(kv => kv.Key).ToList())
                d.Remove(k);
        }
    }
}

