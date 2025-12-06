using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;
using System.Collections.Generic;

// Ensures WidgetManager.Awake() runs before other scripts' Awake
[DefaultExecutionOrder(-100)]
public class WidgetManager : MonoBehaviour {
    private static WidgetManager _instance;
    public static WidgetManager Instance {
        get {
            if (_instance == null) {
                _instance = FindObjectOfType<WidgetManager>();
            }
            return _instance;
        }
        private set { _instance = value; }
    }

    [Tooltip("The RectTransform defining the draggable area")] public RectTransform area;
    private readonly List<WidgetBase> widgets = new List<WidgetBase>();

    void Awake() {
        if (Instance != null && Instance != this) {
            Destroy(gameObject);
            return;
        }
        Instance = this;
    }

    public void Register(WidgetBase w) {
        if (!widgets.Contains(w)) widgets.Add(w);
    }
    public void Unregister(WidgetBase w) {
        widgets.Remove(w);
    }

    // Check if a widget can occupy a given anchored position and size
    public bool CanPlace(WidgetBase w, Vector2 anchoredPos, Vector2 size) {
        RectTransform rt = w.rectTransform;
        Vector2 pivot = rt.pivot;
        Vector2 localBL = anchoredPos - Vector2.Scale(size, pivot);
        Vector2 localTR = localBL + size;
        Vector3 worldBL = rt.parent.TransformPoint(localBL);
        Vector3 worldTR = rt.parent.TransformPoint(localTR);

        Vector3[] areaCorners = new Vector3[4];
        area.GetWorldCorners(areaCorners);
        if (worldBL.x < areaCorners[0].x || worldBL.y < areaCorners[0].y ||
            worldTR.x > areaCorners[2].x || worldTR.y > areaCorners[2].y)
            return false;

        Rect proposed = new Rect(worldBL.x, worldBL.y, worldTR.x - worldBL.x, worldTR.y - worldBL.y);
        foreach (var other in widgets) {
            if (other == w) continue;
            if (proposed.Overlaps(other.GetWorldRect()))
                return false;
        }
        return true;
    }
}