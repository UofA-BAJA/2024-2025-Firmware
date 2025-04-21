using System;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;
using TMPro;
using RoyTheunissen.Graphing;

/// <summary>
/// Attach this to your graph widget prefab.  
/// Requires a GraphComponent and a TMP_Dropdown in the same GameObject hierarchy.
/// </summary>
public class TimeSeriesPlot : MonoBehaviour
{
    [SerializeField] private TMP_Dropdown dropdown;
    private GraphComponent graphComponent;
    private List<DataType> currentOptions = new List<DataType>();
    private DataType selectedType;

    void Awake()
    {
        graphComponent = GetComponentInChildren<GraphComponent>();

        // Populate dropdown initially (may be empty until AvailableDataTypesChanged fires)
        dropdown.ClearOptions();
        dropdown.onValueChanged.AddListener(OnDropdownValueChanged);
    }

    void OnEnable()
    {
        // Subscribe to DataManager events
        DataManager.AvailableDataTypesChanged += UpdateDropdownOptions;
    }

    void OnDisable()
    {
        DataManager.AvailableDataTypesChanged -= UpdateDropdownOptions;
    }

    void Update()
    {

        try{
            var data = DataManager.Instance.GetLatestData(selectedType);
            // The Key is the data, the Value is the timestamp
            graphComponent.Graph.AddValue(data.Value);

        }
        catch (ArgumentException){
            // Do nothing
        }
    }
    private void UpdateDropdownOptions(List<DataType> currentOptions)
    {
        this.currentOptions = currentOptions;
        // Build labels
        var labels = currentOptions.Select(dt => dt.ToString()).ToList();
        dropdown.ClearOptions();
        dropdown.AddOptions(labels);

        // Reset selection to first available
        if (currentOptions.Count > 0)
        {
            dropdown.value = 0;
            selectedType = currentOptions[0];
            graphComponent.Graph.Cleanup();
            graphComponent.Graph.SetRange(0, 0);

        }
    }

    private void OnDropdownValueChanged(int idx)
    {

        if (idx < 0 || idx >= currentOptions.Count)
            return;
        selectedType = currentOptions[idx];
        graphComponent.Graph.Cleanup();
        graphComponent.Graph.SetRange(0, 0);
    }

}
