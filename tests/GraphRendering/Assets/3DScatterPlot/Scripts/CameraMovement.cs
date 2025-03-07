using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraMovement : MonoBehaviour
{

    [SerializeField] float rotSpeed;
    private void Awake()
    {
        
    }



    private void Update()
    {
        transform.Rotate(new Vector3(0, Time.deltaTime * rotSpeed, 0));
    }
}
