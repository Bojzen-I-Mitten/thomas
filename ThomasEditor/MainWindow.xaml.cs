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

using Xceed.Wpf.AvalonDock.Themes;
using System.Reflection;
using System.ComponentModel;
namespace ThomasEditor
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    /// 

    public partial class MainWindow : Window
    {
        TimeSpan lastRender;
        public MainWindow()
        {
           
            InitializeComponent();
            
            CompositionTarget.Rendering += DoUpdates;
            thomasObjects.SelectedItemChanged += ThomasObjects_SelectedItemChanged;

            GameObject.GameObjects.CollectionChanged += GameObjects_CollectionChanged;

        }

        private void GameObjects_CollectionChanged(object sender, System.Collections.Specialized.NotifyCollectionChangedEventArgs e)
        {
            if(e.NewItems != null)
            {
                foreach (GameObject newItem in e.NewItems)
                {
                    if (newItem.transform.parent == null)
                    {
                        TreeViewItem node = new TreeViewItem();
                        node.DataContext = newItem;
                        node.MouseRightButtonUp += Node_MouseRightButtonUp;
                        node.SetBinding(TreeViewItem.HeaderProperty, new Binding("Name"));
                        BuildTree(newItem.transform, node);
                        thomasObjects.Items.Add(node);
                    }
                }
            }
            
            if(e.OldItems != null)
            {
                foreach (GameObject oldItem in e.OldItems)
                {
                    foreach (TreeViewItem node in thomasObjects.Items)
                    {
                        if (node.DataContext == oldItem)
                        {
                            node.MouseRightButtonUp -= Node_MouseRightButtonUp;
                            thomasObjects.Items.Remove(node);
                            break;
                        }
                    }
                }
            }
            
        }

        private void Node_MouseRightButtonUp(object sender, MouseButtonEventArgs e)
        {
            ContextMenu cm = this.FindResource("gameObjectContext") as ContextMenu;
            cm.PlacementTarget = sender as TreeViewItem;
            cm.IsOpen = true;
        }

        private void ThomasObjects_SelectedItemChanged(object sender, RoutedPropertyChangedEventArgs<object> e)
        {
            ItemContainerGenerator gen = thomasObjects.ItemContainerGenerator;
            TreeViewItem item = gen.ContainerFromItem(thomasObjects.SelectedItem) as TreeViewItem;
            __inspector.gameObject = null;
            if (item != null)
            {                
                __inspector.gameObject = ((GameObject)item.DataContext);
            }
            
        }

        private void BuildTree(Transform parent, TreeViewItem parentTree)
        {
            parentTree.IsExpanded = true;
            foreach(Transform child in parent.children)
            {
                TreeViewItem node = new TreeViewItem();
                node.DataContext = child.gameObject;
                node.MouseRightButtonUp += Node_MouseRightButtonUp;
                node.SetBinding(TreeViewItem.HeaderProperty, new Binding("Name"));
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

                List<String> outputs = ThomasWrapper.GetLogOutput();
                if(outputs.Count > 0)
                {
                    foreach (String output in outputs)
                    {
                        TextBlock block = new TextBlock();
                        block.Text = output;
                        block.TextWrapping = TextWrapping.Wrap;
                        console.Items.Add(block);
                        console.Items.Add(new Separator());
                    }
                       
                }
                

                lastRender = args.RenderingTime;
            }
            
        }

        private void AddEmptyGameObject(object sender, RoutedEventArgs e)
        {
            var x = new GameObject("gameObject");
            x.AddComponent<TestComponent>();
        }

        private void Menu_RemoveGameObject(object sender, RoutedEventArgs e)
        {
            var x = sender as MenuItem;
            GameObject.GameObjects.Remove(x.DataContext as GameObject);
        }
    }
}
