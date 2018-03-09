﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ThomasEditor
{
    class TestComponent : ScriptComponent
    {
        public int testVar { get; set; }
        
        
        public override void Update()
        {
            if (Input.GetKeyDown(Input.Keys.Space))
            {
                testVar = 10;
            }
        }
    }
}


