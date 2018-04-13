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

        bool _isDragging;

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
            if (ThomasWrapper.SelectedGameObjects.Contains(parent.gameObject))
                parentTree.IsSelected = true;

            foreach (Transform child in parent.children)
            {
                TreeViewItem node = new TreeViewItem { DataContext = child.gameObject };
               // node.MouseRightButtonUp += Node_MouseRightButtonUp;
                node.SetBinding(TreeViewItem.HeaderProperty, new Binding("Name"));
                node.Padding = new Thickness(0, 0, 0, 2);
                BuildTree(child, node);
                parentTree.Items.Add(node);
            }
        }


        private void ResetTreeView()
        {
            hierarchy.Items.Clear();
            foreach (GameObject gObj in Scene.CurrentScene.GameObjects)
            {
                if (gObj.transform.parent == null)
                {
                    TreeViewItem node = new TreeViewItem { DataContext = gObj };
                    //node.MouseRightButtonUp += Node_MouseRightButtonUp;
                    node.SetBinding(TreeViewItem.HeaderProperty, new Binding("Name"));
                    node.Padding = new Thickness(0, 0, 0, 2);
                    BuildTree(gObj.transform, node);
                    hierarchy.Items.Add(node);
                }
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
                        node.Padding = new Thickness(0, 0, 0, 2);
                        
                       
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



        private void SelectOrDeselectInTree(ItemCollection nodes, System.Collections.IList items, bool select)
        {
            foreach (TreeViewItem node in nodes)
            {
                if(items.Contains(node.DataContext))
                {
                    node.IsSelected = select;
                }
                SelectOrDeselectInTree(node.Items, items, select);
            }
        }

        private void ResetTree(ItemCollection nodes)
        {
            foreach (TreeViewItem node in nodes)
            {
                node.IsSelected = false;
                ResetTree(node.Items);
            }
        }

        private void SceneSelectedGameObjectChanged(object sender, System.Collections.Specialized.NotifyCollectionChangedEventArgs e)
        {
            if (e.NewItems != null)
            {
                SelectOrDeselectInTree(hierarchy.Items, e.NewItems, true);
            }
            if (e.OldItems != null)
            {
                SelectOrDeselectInTree(hierarchy.Items, e.NewItems, false);
            }

            if (e.Action == System.Collections.Specialized.NotifyCollectionChangedAction.Reset)
            {
                ResetTree(hierarchy.Items);
            }
        }


        private void SelectedGameObjectChanged(object sender, RoutedPropertyChangedEventArgs<object> e)
        {
            ItemContainerGenerator gen = hierarchy.ItemContainerGenerator;

            ThomasGameObjectInspector inspector = ((MainWindow)Application.Current.MainWindow).__inspector;
            inspector.SelectedGameObject = null;
            if (hierarchy.SelectedItem != null)
            {
                TreeViewItem item = hierarchy.SelectedItem as TreeViewItem;
                if (item != null)
                {
                    inspector.SelectedGameObject = (GameObject)item.DataContext;
                    if (!ThomasWrapper.SelectedGameObjects.Contains((GameObject)item.DataContext))
                        ThomasWrapper.SelectGameObject((GameObject)item.DataContext);
                }

            }
        }

        private void hierarchy_DragOver(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(typeof(Task)))
            {
                e.Effects = DragDropEffects.Move;
            }
            else
            {
                e.Effects = DragDropEffects.None;
            }
        }

        private TreeViewItem GetNearestContainer(UIElement element)
        {
            // Walk up the element tree to the nearest tree view item.
            TreeViewItem container = element as TreeViewItem;
            while ((container == null) && (element != null))
            {
                element = VisualTreeHelper.GetParent(element) as UIElement;
                container = element as TreeViewItem;
            }
            return container;
        }

        TreeViewItem GetItemAtLocation(Point location)
        {
            TreeViewItem foundItem = default(TreeViewItem);
            HitTestResult hitTestResults = VisualTreeHelper.HitTest(hierarchy, location);
            if (hitTestResults != null && hitTestResults.VisualHit is FrameworkElement)
            {
                foundItem = GetNearestContainer(hitTestResults.VisualHit as FrameworkElement);
            }

            return foundItem;
        }

        private void hierarchy_Drop(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(typeof(TreeViewItem)))
            {
                
                TreeViewItem source = (TreeViewItem)e.Data.GetData(typeof(TreeViewItem));
                TreeViewItem target = GetItemAtLocation(e.GetPosition(hierarchy));

                if(target != null && source != null && target != source)
                {
                    GameObject parent = target.DataContext as GameObject;
                    GameObject child = source.DataContext as GameObject;
                    if(!parent.transform.IsChildOf(child.transform))
                    {
                        child.transform.parent = parent.transform;
                        ResetTreeView();
                    }
                }
                else if(source != null && target == null)
                {
                    GameObject child = source.DataContext as GameObject;
                    child.transform.parent = null;
                    ResetTreeView();
                }

                // Code to move the item in the model is placed here...
            }
        }

        private void hierarchy_MouseMove(object sender, MouseEventArgs e)
        {
            if (!_isDragging && e.LeftButton == MouseButtonState.Pressed && hierarchy.SelectedValue != null)
            {
                _isDragging = true;
                DragDrop.DoDragDrop(hierarchy, hierarchy.SelectedValue,
                    DragDropEffects.Move);
            }
            else if(e.LeftButton != MouseButtonState.Pressed)
            {
                _isDragging = false;
                
            }

        }

        private void hierarchy_MouseDown(object sender, MouseButtonEventArgs e)
        {
        }
    }
}
