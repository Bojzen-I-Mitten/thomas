using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ThomasEditor
{
    public class TestComponent : ScriptComponent
    {
        public Curve posCurve {get; set;}
        public Curve scaleCurve { get; set; }

        float t = 0;

        public override void Start()
        {

        }

        public override void Update()
        {
            t += Time.DeltaTime;
            gameObject.transform.position = new Vector3(0, (float)posCurve.GetYFromX((t*6.5) % 5), 0);
            gameObject.transform.scale = new Vector3((float)scaleCurve.GetYFromX((t*6.5) % 5), 1, (float)scaleCurve.GetYFromX((t*6.5) % 5));
        }
    }
}


