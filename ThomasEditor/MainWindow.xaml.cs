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

using System.Reflection;
using System.ComponentModel;
using System.Windows.Threading;

namespace ThomasEditor
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    /// 

    public partial class MainWindow : Window
    {
        double scrollRatio = 0;
        TimeSpan lastRender;
        public MainWindow()
        {

            InitializeComponent();

            //Changeds decimals to . instead of ,
            System.Globalization.CultureInfo customCulture = (System.Globalization.CultureInfo)System.Threading.Thread.CurrentThread.CurrentCulture.Clone();
            customCulture.NumberFormat.NumberDecimalSeparator = ".";

            System.Threading.Thread.CurrentThread.CurrentCulture = customCulture;


            CompositionTarget.Rendering += DoUpdates;

            thomasObjects.SelectedItemChanged += ThomasObjects_SelectedItemChanged;


            Scene.CurrentScene.GameObjects.CollectionChanged += GameObjects_CollectionChanged;
            ThomasWrapper.OutputLog.CollectionChanged += OutputLog_CollectionChanged;
            ThomasWrapper.SelectedGameObjects.CollectionChanged += SelectedGameObjects_CollectionChanged;
        }

        private void SelectedGameObjects_CollectionChanged(object sender, System.Collections.Specialized.NotifyCollectionChangedEventArgs e)
        {
            if (e.NewItems != null)
            {
                foreach (TreeViewItem node in thomasObjects.Items)
                {
                    if (node.DataContext == (GameObject)e.NewItems[0])
                    {
                        node.IsSelected = true;
                        break;
                    }
                }
                __inspector.SelectedGameObject = (GameObject)e.NewItems[0];
            }
            if(e.OldItems != null)
            {
                foreach(GameObject gObj in e.OldItems)
                {
                    foreach (TreeViewItem node in thomasObjects.Items)
                    {
                        if (node.DataContext == gObj)
                        {
                            node.IsSelected = false;
                            break;
                        }
                    }
                }
            }

            if(e.Action == System.Collections.Specialized.NotifyCollectionChangedAction.Reset)
            {
                foreach (TreeViewItem node in thomasObjects.Items)
                {
                    node.IsSelected = false;
                }
            }
        }
        private void OutputLog_CollectionChanged(object sender, System.Collections.Specialized.NotifyCollectionChangedEventArgs e)
        {
            if (e.NewItems != null)
            {
                foreach (String output in e.NewItems)
                {
                    TextBlock block = new TextBlock
                    {
                        Text = output,
                        TextWrapping = TextWrapping.Wrap
                    };
                    console.Items.Add(block);
                    console.Items.Add(new Separator());
                    if (console.Items.Count > 10)
                        console.Items.RemoveAt(0);
                }
            }
        }

        private void GameObjects_CollectionChanged(object sender, System.Collections.Specialized.NotifyCollectionChangedEventArgs e)
        {
            if (e.NewItems != null)
            {

                foreach (GameObject newItem in e.NewItems)
                {
                    if (newItem.transform.parent == null)
                    {
                        TreeViewItem node = new TreeViewItem { DataContext = newItem};
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
                    oldItem.Destroy();
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
            __inspector.SelectedGameObject = null;
            if (item != null)
            {
                __inspector.SelectedGameObject = (GameObject)item.DataContext;
                ThomasWrapper.SelectGameObject((GameObject)item.DataContext);
            }
            
        }

        private void BuildTree(Transform parent, TreeViewItem parentTree)
        {
            parentTree.IsExpanded = true;
            foreach(Transform child in parent.children)
            {
                TreeViewItem node = new TreeViewItem {DataContext = child.gameObject};
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
                editorWindow.Title = ThomasWrapper.FrameRate.ToString();
               lastRender = args.RenderingTime;
             }
            
        }

        private void NewEmptyObject_CanExecute(object sender, CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = true;
        }

        private void AddEmptyGameObject(object sender, RoutedEventArgs e)
        {
            var x = new GameObject("gameObject");
        }

        private void OpenOptionsWindow_CanExecute(object sender, CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = true;
        }

        private void OpenOptionsWindow(object sender, RoutedEventArgs e)
        {
            OptionsWindow oW = new OptionsWindow();
            //oW.Show();
            oW.ShowDialog();
            oW.Focus();
        }

        private void Menu_RemoveGameObject(object sender, RoutedEventArgs e)
        {
            var x = sender as MenuItem;
           // Scene.CurrentScene.GameObjects.Remove(x.DataContext as GameObject);
        }

        private void LoadWaveFile(object sender, RoutedEventArgs e)
        {
           // var x = new 
        }

        private void Console_ScrollChanged(object sender, ScrollChangedEventArgs e)
        {

            ScrollViewer scrollViewer = Extensions.GetDescendantByType<ScrollViewer>(sender as ListBox);

            if (e.ExtentWidthChange != 0 || e.ExtentHeightChange != 0)
            {
                //calculate and set accordingly
                double offset = scrollRatio * e.ExtentHeight - 0.5 * e.ViewportHeight;
                //see if it is negative because of initial values
                if (offset < 0)
                {
                    //center the content
                    //this can be set to 0 if center by default is not needed
                    offset = 0.5 * scrollViewer.ScrollableHeight;
                }
                scrollViewer.ScrollToVerticalOffset(offset);
            }
            else
            {
                //store the relative values if normal scroll
                if(e.ExtentHeight > 0)
                    scrollRatio = (e.VerticalOffset + 0.5 * e.ViewportHeight) / e.ExtentHeight;
            }


        }

        private void Recompile_Shader_Click(object sender, RoutedEventArgs e)
        {
            Shader.RecompileShaders();
        }

        private void PlayPauseButton_Click(object sender, RoutedEventArgs e)
        {
            if(ThomasWrapper.IsPlaying())
            {

            }
            ThomasWrapper.Play();
        }
    }

    public static class Extensions
    {

        public static List<Type> GetAllComponentTypes()
        {

            Type componenType = typeof(Component);
            List<Type> componenTypesFromEngine = Assembly.GetAssembly(componenType).GetTypes()
                .Where(t =>
                t != componenType &&
                t != typeof(ScriptComponent) &&
                t != typeof(Transform) &&
                componenType.IsAssignableFrom(t)
                ).ToList();

            List<Type> componentTypesFromEditor = Assembly.GetExecutingAssembly().GetTypes()
                .Where(t =>
                t != componenType &&
                componenType.IsAssignableFrom(t)
                ).ToList();

            List<Type> allComponentTypes = new List<Type>();
            allComponentTypes.AddRange(componenTypesFromEngine);
            allComponentTypes.AddRange(componentTypesFromEditor);

            return allComponentTypes;
        }

        public static T GetDescendantByType<T>(this Visual element) where T : class
        {
            if (element == null)
            {
                return default(T);
            }
            if (element.GetType() == typeof(T))
            {
                return element as T;
            }
            T foundElement = null;
            if (element is FrameworkElement)
            {
                (element as FrameworkElement).ApplyTemplate();
            }
            for (var i = 0; i < VisualTreeHelper.GetChildrenCount(element); i++)
            {
                var visual = VisualTreeHelper.GetChild(element, i) as Visual;
                foundElement = visual.GetDescendantByType<T>();
                if (foundElement != null)
                {
                    break;
                }
            }
            return foundElement;
        }
    }
}
