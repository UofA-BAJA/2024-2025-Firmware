using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;

public class Temporary : MonoBehaviour
{
    [SerializeField] DataManager dataManager;
    [SerializeField] private TMP_Dropdown dataTypes;

    void Update()
    {
        dataTypes.ClearOptions();


        List<string> dataTypeStrings = new();
        foreach(DataType dataType in dataManager.GetAvaiableDataTypes()){
            dataTypeStrings.Add(dataType.ToString());
        }


        dataTypes.AddOptions(dataTypeStrings);
    }
}
