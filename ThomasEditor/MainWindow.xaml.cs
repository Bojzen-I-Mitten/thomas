using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Interop;



namespace ThomasEditor
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    /// 

    public partial class MainWindow : Window
    {
        TimeSpan lastRender;
        int lastCount = 0;

        public MainWindow()
        {
            InitializeComponent();

            CompositionTarget.Rendering += DoUpdates;

        }


        private void BuildTree(Transform parent, TreeViewItem parentTree)
        {
            parentTree.IsExpanded = true;
            foreach(Transform child in parent.children)
            {
                TreeViewItem node = new TreeViewItem();
                node.Header = child.gameObject.GetType();
                BuildTree(child, node);
                parentTree.Items.Add(node);


            }
        }


        private void DoUpdates(object sender, EventArgs e)
        {

            
            RenderingEventArgs args = (RenderingEventArgs)e;
            
            if(this.lastRender != args.RenderingTime)
            {
                ThomasWrapper.Update();
                var objects = GameObject.GetGameObjects();
                
                if(objects.Count != lastCount)
                {
                    thomasObjects.Items.Clear();
                    objects.ForEach(delegate (GameObject g) {
                        if (g.transform.parent == null)
                        {
                            TreeViewItem node = new TreeViewItem();
                            node.Header = g.GetType();
                            BuildTree(g.transform, node);

                            thomasObjects.Items.Add(node);
                        }
                    });
                    lastCount = objects.Count();
                }

                List<String> outputs = ThomasWrapper.GetLogOutput();
                if(outputs.Count > 0)
                {
                    foreach (String output in outputs)
                        console.Items.Add(output);
                }
                

                lastRender = args.RenderingTime;
            }
           
            
        }
    }
}
