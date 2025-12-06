using UnityEngine;
using UnityEngine.EventSystems;
// Attach to the bottom-left handle: enables resizing
public class ResizeHandle : MonoBehaviour, IPointerDownHandler, IDragHandler {
    private WidgetBase widget;
    private RectTransform parentRt;
    private Vector2 originalPointer, originalSize;

    void Awake() {
        widget = GetComponentInParent<WidgetBase>();
        parentRt = widget.rectTransform.parent as RectTransform;
    }

    public void OnPointerDown(PointerEventData eventData) {
        RectTransformUtility.ScreenPointToLocalPointInRectangle(
            parentRt, eventData.position, eventData.pressEventCamera, out originalPointer);
        originalSize = widget.rectTransform.sizeDelta;
    }

    public void OnDrag(PointerEventData eventData) {
        RectTransformUtility.ScreenPointToLocalPointInRectangle(
            parentRt, eventData.position, eventData.pressEventCamera, out Vector2 localPoint);
        Vector2 delta = localPoint - originalPointer;
        Vector2 newSize = originalSize + new Vector2(-delta.x, -delta.y);
        newSize = new Vector2(Mathf.Max(newSize.x, 50), Mathf.Max(newSize.y, 50));
        widget.TryResize(newSize);
    }
}
