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
        
        
        public override void Update()
        {
            if (Input.GetKeyDown(Input.Keys.K))
            {

                RenderComponent r = gameObject.AddComponent<RenderComponent>();
                if (r)
                    r.Model = "testModel";
                
            }
            if (Input.GetKeyDown(Input.Keys.Space))
            {
                testVar = 10;
                //gameObject.GetComponent<SoundComponent>().Play();
                
                
            }
        }
    }
}


