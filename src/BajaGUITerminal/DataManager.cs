// DataManager.cs  (no Unity dependencies)
using System;
using System.Collections.Generic;
using System.Linq;
using Timer = System.Timers.Timer;
namespace Baja.TerminalTelemetry
{
    public class DataManager
    {
        public event Action<DataType, float, float> SampleReady; // (type, t, value)

        private readonly SerialInterface serial;
        private readonly Dictionary<DataType, DataStream> streams = new();
        private readonly Dictionary<DataType, float> freshness = new(); // seconds since last sample

        private readonly Timer pump;      // drives Update() at 60 Hz
        private readonly Timer freshnessTimer;

        public DataManager(string port, int baud)
        {
            serial = new SerialInterface(port, baud);

            for (int i = 0; i < 32; i++)
                streams[(DataType)i] = new DataStream((DataType)i, string.Empty, string.Empty);

            // main update 60 Hz
            pump = new Timer(1000.0 / 30);
            pump.Elapsed += (_, __) => Update();
            pump.AutoReset = true; pump.Start();

            // freshness bookkeeping 20 Hz
            freshnessTimer = new Timer(50);
            freshnessTimer.Elapsed += (_, __) =>
            {
                var expired = freshness
                    .Where(kv => kv.Value > 0.5f)
                    .Select(kv => kv.Key)
                    .ToList();
                foreach (var dt in expired) freshness.Remove(dt);

                foreach (var dt in freshness.Keys.ToList())
                    freshness[dt] += 0.05f;
            };
            freshnessTimer.AutoReset = true; freshnessTimer.Start();
        }

        private void Update()
        {
            var packet = serial.GetData();
            if (packet == null || packet.dataMask == 0) return;

            int mask = packet.dataMask;
            int idx = 0;

            for (int bit = 0; bit < 32 && idx < 6; bit++, mask >>= 1)
            {
                if ((mask & 1) == 0) continue;

                var dt = (DataType)bit;
                float value = packet.data[idx++];
                float t = packet.timestamp;

                streams[dt].AddDataToStream(value, t);
                freshness[dt] = 0f;

                // fire event
                SampleReady?.Invoke(dt, t, value);

            }
        }


	public void SendCommand(Command cmd) => serial.SendCommand(cmd);
    
    }

}

