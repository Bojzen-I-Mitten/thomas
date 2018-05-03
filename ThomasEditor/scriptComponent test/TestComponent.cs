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

        public Thomas_Graph.Bezier curve {get; set;}

        public override void Start()
        {

        }

        public override void Update()
        {
            
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


