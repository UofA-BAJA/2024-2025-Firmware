using UnityEngine;

/// Registers in Start() to avoid any Awake-order dependencies
[DefaultExecutionOrder(-50)]
[RequireComponent(typeof(RectTransform))]
public class WidgetBase : MonoBehaviour {
    [HideInInspector] public RectTransform rectTransform;
    private Vector2 lastValidPos, lastValidSize;
    private bool isRegistered;

    void Awake() {
        rectTransform = GetComponent<RectTransform>();
        lastValidPos = rectTransform.anchoredPosition;
        lastValidSize = rectTransform.sizeDelta;
    }

    void Start() {
        if (WidgetManager.Instance == null) {
            Debug.LogError("No WidgetManager found in scene!");
            return;
        }
        WidgetManager.Instance.Register(this);
        isRegistered = true;
    }

    void OnDestroy() {
        if (isRegistered && WidgetManager.Instance != null)
            WidgetManager.Instance.Unregister(this);
    }

    public Rect GetWorldRect() {
        Vector3[] corners = new Vector3[4];
        rectTransform.GetWorldCorners(corners);
        Vector2 bl = corners[0];
        Vector2 tr = corners[2];
        return new Rect(bl, tr - bl);
    }

    public void TryMove(Vector2 anchoredPos) {
        Vector2 size = rectTransform.sizeDelta;
        if (WidgetManager.Instance.CanPlace(this, anchoredPos, size)) {
            rectTransform.anchoredPosition = anchoredPos;
            lastValidPos = anchoredPos;
        } else {
            rectTransform.anchoredPosition = lastValidPos;
        }
    }

    public void TryResize(Vector2 size) {
        Vector2 pos = rectTransform.anchoredPosition;
        if (WidgetManager.Instance.CanPlace(this, pos, size)) {
            rectTransform.sizeDelta = size;
            lastValidSize = size;
        } else {
            rectTransform.sizeDelta = lastValidSize;
        }
    }
}