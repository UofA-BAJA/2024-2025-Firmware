using UnityEngine;
using UnityEngine.EventSystems;

// Attach to the top handle: enables dragging
public class DragHandle : MonoBehaviour, IPointerDownHandler, IDragHandler {
    private WidgetBase widget;
    private RectTransform parentRt;
    private Vector2 pointerOffset;

    void Awake() {
        widget = GetComponentInParent<WidgetBase>();
        parentRt = widget.rectTransform.parent as RectTransform;
    }

    public void OnPointerDown(PointerEventData eventData) {
        RectTransformUtility.ScreenPointToLocalPointInRectangle(
            parentRt, eventData.position, eventData.pressEventCamera, out Vector2 localPoint);
        pointerOffset = localPoint - widget.rectTransform.anchoredPosition;
    }

    public void OnDrag(PointerEventData eventData) {
        RectTransformUtility.ScreenPointToLocalPointInRectangle(
            parentRt, eventData.position, eventData.pressEventCamera, out Vector2 localPoint);
        Vector2 newPos = localPoint - pointerOffset;
        widget.TryMove(newPos);
    }
}
