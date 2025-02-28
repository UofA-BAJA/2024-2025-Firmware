using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;

public class GraphGridController : MonoBehaviour
{

    [SerializeField] LayerMask graphLayer;

    GameObject selectedGraph;

    private void Awake()
    {

    }

    private void Update()
    {

        if (Input.GetKeyDown(KeyCode.Mouse0))
        {
            PointerEventData pointerEventData = new PointerEventData(EventSystem.current);

            pointerEventData.position = Input.mousePosition;

        
            List<RaycastResult> results = new List<RaycastResult>();

            EventSystem.current.RaycastAll(pointerEventData, results);

            foreach(RaycastResult result in results)
            {
                if((graphLayer & (1 << result.gameObject.layer)) != 0)
                {
                    selectedGraph = result.gameObject;
                }

            }
        }

    }



}
