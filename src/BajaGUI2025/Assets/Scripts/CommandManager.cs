using System;
using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;

public class CommandManager : MonoBehaviour
{
    [SerializeField] private DataManager dataManager;
    [SerializeField] private TMP_Dropdown commandPicker;

    private HashSet<Command> specialCommands = new() { Command.DEFAULT_CAR_START, Command.EMERGENCY_STOP, Command.START_LOG, Command.END_LOG};

    void Awake()
    {
        commandPicker.ClearOptions();
        List<string> dropdownOptions = new List<string>();
        foreach(Command command in Enum.GetValues(typeof(Command))){

            // Don't put the special commands in the dropdown.
            if(specialCommands.Contains(command)){
                continue;
            }

            dropdownOptions.Add(command.ToString());
        }

        commandPicker.AddOptions(dropdownOptions);

    }

    public void SendCommand(){

        string currVal = commandPicker.options[commandPicker.value].text;

        Command currCommand;
        Enum.TryParse(currVal, out currCommand);

        dataManager.SendCommand(currCommand);
    }

    public void EmergencyStop(){
        dataManager.SendCommand(Command.EMERGENCY_STOP);
    }

    public void StartLog(){
        dataManager.SendCommand(Command.START_LOG);
    }

    public void EndLog(){
        dataManager.SendCommand(Command.END_LOG);
    }
}
