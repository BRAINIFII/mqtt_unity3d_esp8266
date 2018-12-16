using UnityEngine;
using System.Collections;
using System.Net;
using uPLibrary.Networking.M2Mqtt;
using uPLibrary.Networking.M2Mqtt.Messages;
using uPLibrary.Networking.M2Mqtt.Utility;
using uPLibrary.Networking.M2Mqtt.Exceptions;
using UnityEngine.UI;
using System;

public class mqttTest : MonoBehaviour {
	private MqttClient client;
    public int connect = 0;
    public string Kuid="remotecontrollerid"; // myphone, mypc, carcommander, servocontroller, jhon, etc. (you will setting a name (String), device on running in unity)
    public string Arid="";
    public string Aridtmp;
    public int Asama=0;
    public int i;
    public string Recv = "";
    public string ERecv = "";
    public string Kuidtmp;
    public string Komutlar;
    public float gzaman=0;
    public int Servo01deg, oldServo01deg;
    public int Servo02deg, oldServo02deg;

    // Use this for initialization
    void Start () {
            Arid ="" ;

        // create client instance 
        //zyxel client = new MqttClient(IPAddress.Parse("192.168.2.2"), 1883, false, null);
        client = new MqttClient(IPAddress.Parse("192.168.2.2"), 1883, false, null);
        // register to message received 
        client.MqttMsgPublishReceived += Client_MqttMsgPublishReceived;

        string clientId = Guid.NewGuid().ToString();
        client.Connect(clientId);

        // subscribe to the topic "/home/temperature" with QoS 2 
        client.Subscribe(new string[] { "/HOME/KEP/OY" }, new byte[] { MqttMsgBase.QOS_LEVEL_AT_MOST_ONCE });

        client.Publish("/HOME/KEP/KU", System.Text.Encoding.UTF8.GetBytes("<" + Kuid + "=Myid?;|"), MqttMsgBase.QOS_LEVEL_AT_MOST_ONCE, false);
    }

    void Client_MqttMsgPublishReceived(object sender, MqttMsgPublishEventArgs e)
    {
        Debug.Log("Received: " + System.Text.Encoding.UTF8.GetString(e.Message));
        Recv = System.Text.Encoding.UTF8.GetString(e.Message);

    }

    void OnGUI(){

        if (Arid == "" & connect==0)
        {
            // araç id isteme
            if (GUI.Button(new Rect(10, 20, 100, 30), "Id sor"))
            {
                Arid = "";

                Debug.Log("sending...");
                client.Publish("/HOME/KEP/KU", System.Text.Encoding.UTF8.GetBytes(">"+ Kuid + "=Myid?;|"), MqttMsgBase.QOS_LEVEL_AT_MOST_ONCE, false);
                // client.Publish("/Test/1", System.Text.Encoding.UTF8.GetBytes("S f U" + count.ToString()), MqttMsgBase.QOS_LEVEL_AT_MOST_ONCE, false);
                Debug.Log("sent");
            }

        }

        //if (Recv.Length < 2) Recv = "";

        if(Recv.Length > 2) { 

            if (Recv != "" & ERecv!=Recv)
            {

                if (Arid == "" & connect == 0)
                {


                    /*  if (Recv.IndexOf("<")>=0) {
                        Debug.Log(Recv);
                        Recv = Recv.Substring(Recv.IndexOf("<")+1, Recv.Length - (Recv.IndexOf("<")- Recv.IndexOf("<") + 1));
                        } id sormadan connect çıkması için */


                    Kuidtmp = Recv.Substring(0, Recv.IndexOf("="));
                    if (Kuidtmp == Kuid)
                    {
                        Debug.Log(Recv);
                        Debug.Log(Recv.Length);
                        ERecv = Recv;
                        Debug.Log("start:" + Recv.IndexOf("=<"));
                        Debug.Log("Finish:" +  Recv.IndexOf(";"));
                        Arid = Recv.Substring((Recv.IndexOf("=<") + 2), (Recv.Length+3)-Recv.IndexOf(";"));
                        Recv = "";

                    }

                }
            }
        }

        if (Arid != "" & connect == 0)
        {
            if (GUI.Button(new Rect(50, 20, 200, 40), Arid + " Connect"))
            {
                Debug.Log("sending...");
                client.Publish("/HOME/KEP/KU", System.Text.Encoding.UTF8.GetBytes(">" + Kuid + "==" + Arid + ";|"), MqttMsgBase.QOS_LEVEL_AT_MOST_ONCE, false);
                // client.Publish("/Test/1", System.Text.Encoding.UTF8.GetBytes("S f U" + count.ToString()), MqttMsgBase.QOS_LEVEL_AT_MOST_ONCE, false);
                Debug.Log("sent");
                connect = 1; ERecv = "";
            }
        }

        if (Arid != "" & connect == 1)
        {
            if (GUI.Button(new Rect(0, 20, 200, 40), Arid + " Disconnect"))
            {
                Debug.Log("sending...");
                client.Publish("/HOME/KEP/KU", System.Text.Encoding.UTF8.GetBytes(">" + Kuid + "=" + Arid + ";" + "KUIDKILL=" + ".." + ";" + "|"), MqttMsgBase.QOS_LEVEL_AT_MOST_ONCE, false);
                // client.Publish("/Test/1", System.Text.Encoding.UTF8.GetBytes("S f U" + count.ToString()), MqttMsgBase.QOS_LEVEL_AT_MOST_ONCE, false);
                Debug.Log("sent");
                connect = 1; Arid = "";ERecv = "";
            }
        }

            /*
                    if (GUI.Button(new Rect(20, 200, 200, 40), Arid + " 90"))
                    {
                        Debug.Log("sending...");
                        client.Publish("/HOME/KEP/KU", System.Text.Encoding.UTF8.GetBytes(">" + Kuid + "=" + Arid + ";"+"SERVO=90;"+"|"), MqttMsgBase.QOS_LEVEL_AT_MOST_ONCE, false);
                        // client.Publish("/Test/1", System.Text.Encoding.UTF8.GetBytes("S f U" + count.ToString()), MqttMsgBase.QOS_LEVEL_AT_MOST_ONCE, false);
                        Debug.Log("sent");
                    }
                    if (GUI.Button(new Rect(20, 300, 200, 40), Arid + " 45"))
                    {
                        Debug.Log("sending...");
                        client.Publish("/HOME/KEP/KU", System.Text.Encoding.UTF8.GetBytes(">" + Kuid + "=" + Arid + ";" + "SERVO=45;" + "|"), MqttMsgBase.QOS_LEVEL_AT_MOST_ONCE, false);
                        // client.Publish("/Test/1", System.Text.Encoding.UTF8.GetBytes("S f U" + count.ToString()), MqttMsgBase.QOS_LEVEL_AT_MOST_ONCE, false);
                        Debug.Log("sent");
                    }
                    */
    }


    // Update is called once per frame
    void FixedUpdate ()
    {
        Komutlar = ""; 
        if (Arid != "" & connect == 1)
        {
            gzaman += Time.fixedDeltaTime;
            Slider mysli1 = GameObject.Find("Slider1").GetComponent<Slider>();
            Slider mysli2 = GameObject.Find("Slider2").GetComponent<Slider>();
            if (Servo01deg != (int)mysli1.value)
            {
                Servo01deg = (int)mysli1.value;
                //client.Publish("/HOME/KEP/KU", System.Text.Encoding.UTF8.GetBytes(">" + Kuid + "=" + Arid + ";" + "SERVO=" + Servo01deg.ToString() + ";" + "|"), MqttMsgBase.QOS_LEVEL_AT_MOST_ONCE, false);
                gzaman = 0;
            }
            if (Servo02deg != (int)mysli2.value)
            {
                Servo02deg = (int)mysli2.value;
                //client.Publish("/HOME/KEP/KU", System.Text.Encoding.UTF8.GetBytes(">" + Kuid + "=" + Arid + ";" + "SERVO=" + Servo01deg.ToString() + ";" + "|"), MqttMsgBase.QOS_LEVEL_AT_MOST_ONCE, false);
                gzaman = 0;
            }

            if (gzaman >= 0.05f)
            {
                if (oldServo01deg != Servo01deg)
                {
                    Komutlar += "SERVO01=" + Servo01deg.ToString() + ";";
                    oldServo01deg = Servo01deg;
                }
                if (oldServo02deg != Servo02deg)
                {
                    Komutlar += "SERVO02=" + Servo02deg.ToString() + ";";
                    oldServo02deg = Servo02deg;

                }

                if (Komutlar!="") client.Publish("/HOME/KEP/KU", System.Text.Encoding.UTF8.GetBytes(">" + Kuid + "=" + Arid + ";" + Komutlar + "|"), MqttMsgBase.QOS_LEVEL_AT_MOST_ONCE, false);
                gzaman = 0;
            }

        }
    }


    public void gonderservo01()
    {
        Slider mysli1 = GameObject.Find("Slider1").GetComponent<Slider>();
            Servo01deg = (int)mysli1.value;
    }
    public void gonderservo02()
    {
        Slider mysli2 = GameObject.Find("Slider2").GetComponent<Slider>();
        Servo02deg = (int)mysli2.value;
    }
}
