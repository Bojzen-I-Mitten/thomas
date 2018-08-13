using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ThomasEditor
{
    public class TestComponent : ScriptComponent
    {
        public int testVar { get; set; }
        public AudioClip audioClip { get; set; }
        public Material mat { get; set; }
        public Model m { get; set; }

        public Thomas_Graph.Bezier posCurve {get; set;}
        public Thomas_Graph.Bezier scaleCurve { get; set; }

        float t = 0;

        public override void Start()
        {
            m.GenerateBones(gameObject);
        }

        public override void Update()
        {
            t += Time.DeltaTime;
            gameObject.transform.position = new Vector3(0, (float)posCurve.GetYFromX((t*10) % 10), 0);
            gameObject.transform.scale = new Vector3((float)scaleCurve.GetYFromX((t*10) % 10), 1, (float)scaleCurve.GetYFromX((t*10) % 10));
            if (Input.GetKeyDown(Input.Keys.K))
            {

                //RenderComponent r = gameObject.AddComponent<RenderComponent>();
                
            }
            if (Input.GetKeyDown(Input.Keys.Space))
            {
                testVar = 10;
                //gameObject.GetComponent<SoundComponent>().Play();
                
                
            }
        }
    }
}


