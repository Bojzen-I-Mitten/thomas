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

namespace ThomasEditor
{
    /// <summary>
    /// Interaction logic for GameObjectHierarchy.xaml
    /// </summary>
    public partial class GameObjectHierarchy : UserControl
    {
        public GameObjectHierarchy()
        {
            InitializeComponent();
            Scene.sceneChanged = SceneGameObjectsChanged;
            Scene.CurrentScene.GameObjects.CollectionChanged += SceneGameObjectsChanged;
            ThomasWrapper.SelectedGameObjects.CollectionChanged += SceneSelectedGameObjectChanged;
        }

        private void BuildTree(Transform parent, TreeViewItem parentTree)
        {
            parentTree.IsExpanded = true;
            foreach (Transform child in parent.children)
            {
                TreeViewItem node = new TreeViewItem { DataContext = child.gameObject };
               // node.MouseRightButtonUp += Node_MouseRightButtonUp;
                node.SetBinding(TreeViewItem.HeaderProperty, new Binding("Name"));
                BuildTree(child, node);
                parentTree.Items.Add(node);
            }
        }


        private void SceneGameObjectsChanged(object sender, System.Collections.Specialized.NotifyCollectionChangedEventArgs e)
        {
            if (e.NewItems != null)
            {

                foreach (GameObject newItem in e.NewItems)
                {
                    if (newItem.transform.parent == null)
                    {
                        TreeViewItem node = new TreeViewItem { DataContext = newItem };
                        //node.MouseRightButtonUp += Node_MouseRightButtonUp;
                        node.SetBinding(TreeViewItem.HeaderProperty, new Binding("Name"));
                        BuildTree(newItem.transform, node);
                        hierarchy.Items.Add(node);
                    }
                }
            }

            if (e.OldItems != null)
            {
                foreach (GameObject oldItem in e.OldItems)
                {
                    //oldItem.Destroy();
                    foreach (TreeViewItem node in hierarchy.Items)
                    {
                        if (node.DataContext == oldItem)
                        {
                            //node.MouseRightButtonUp -= Node_MouseRightButtonUp;
                            hierarchy.Items.Remove(node);
                            break;
                        }
                    }
                }
            }

        }
        private void SceneSelectedGameObjectChanged(object sender, System.Collections.Specialized.NotifyCollectionChangedEventArgs e)
        {
            if (e.NewItems != null)
            {
                foreach (TreeViewItem node in hierarchy.Items)
                {
                    if (node.DataContext == (GameObject)e.NewItems[0])
                    {
                        node.IsSelected = true;
                        break;
                    }
                }
               // __inspector.SelectedGameObject = (GameObject)e.NewItems[0];
            }
            if (e.OldItems != null)
            {
                foreach (GameObject gObj in e.OldItems)
                {
                    foreach (TreeViewItem node in hierarchy.Items)
                    {
                        if (node.DataContext == gObj)
                        {
                            node.IsSelected = false;
                            break;
                        }
                    }
                }
            }

            if (e.Action == System.Collections.Specialized.NotifyCollectionChangedAction.Reset)
            {
                foreach (TreeViewItem node in hierarchy.Items)
                {
                    if (node.IsSelected)
                        node.IsSelected = false;
                }
            }
        }


        private void SelectedGameObjectChanged(object sender, RoutedPropertyChangedEventArgs<object> e)
        {
            ItemContainerGenerator gen = hierarchy.ItemContainerGenerator;

            ThomasGameObjectInspector inspector = ((MainWindow)Application.Current.MainWindow).__inspector;
            inspector.SelectedGameObject = null;
            if (hierarchy.SelectedItem != null)
            {
                TreeViewItem item = gen.ContainerFromItem(hierarchy.SelectedItem) as TreeViewItem;
                if (item != null)
                {
                    inspector.SelectedGameObject = (GameObject)item.DataContext;
                    if (!ThomasWrapper.SelectedGameObjects.Contains((GameObject)item.DataContext))
                        ThomasWrapper.SelectGameObject((GameObject)item.DataContext);
                }
            }
        }

    }
}
