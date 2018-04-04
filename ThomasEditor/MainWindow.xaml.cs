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
using System.Xml.Serialization;
using System.Reflection;
using System.ComponentModel;
using System.Windows.Threading;

using System.Runtime.Serialization;
using System.IO;
using System.Security.Policy;

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


            Component.editorAssembly = Assembly.GetAssembly(this.GetType());

            //Changeds decimals to . instead of ,
            System.Globalization.CultureInfo customCulture = (System.Globalization.CultureInfo)System.Threading.Thread.CurrentThread.CurrentCulture.Clone();
            customCulture.NumberFormat.NumberDecimalSeparator = ".";

            System.Threading.Thread.CurrentThread.CurrentCulture = customCulture;


            CompositionTarget.Rendering += DoUpdates;

      
            ThomasWrapper.OutputLog.CollectionChanged += OutputLog_CollectionChanged;
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


        private void Node_MouseRightButtonUp(object sender, MouseButtonEventArgs e)
        {
            ContextMenu cm = this.FindResource("gameObjectContext") as ContextMenu;
            cm.PlacementTarget = sender as TreeViewItem;
            cm.IsOpen = true;
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
            ThomasWrapper.SelectGameObject(x);
        }

        private void OpenOptionsWindow_CanExecute(object sender, CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = true;
        }

        private void OpenOptionsWindow(object sender, RoutedEventArgs e)
        {
            OptionsWindow oW = new OptionsWindow();
            oW.ShowDialog();
            oW.Focus();
        }

        private void Menu_RemoveGameObject(object sender, RoutedEventArgs e)
        {
            var x = sender as MenuItem;
            (x.DataContext as GameObject).Destroy();
        }

        private void RemoveSelectedGameObjects_CanExecute(object sender, CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = true;
        }

        private void RemoveSelectedGameObjects(object sender, RoutedEventArgs e)
        {
            for(int i=0; i < ThomasWrapper.SelectedGameObjects.Count; i++)
            {
                GameObject gObj = ThomasWrapper.SelectedGameObjects[i];
                gObj.Destroy();
                ThomasWrapper.SelectedGameObjects.RemoveAt(i);
               i--;
            }
        }

        private void LoadWaveFile(object sender, RoutedEventArgs e)
        {
            var x = ThomasEditor.Resources.Load<AudioClip>("..\\Data\\test.wav");
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

        private void Play_CanExecute(object sender, CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = true;
        }

        private void SaveScene_Click(object sender, RoutedEventArgs e)
        {

            if(Scene.CurrentScene.HasFile)
            {
                Scene sceneToSave = Scene.CurrentScene;
                Scene.SaveScene(sceneToSave);
            }
            else
            {
                SaveSceneAs_Click(sender, e);
            }
        }


        private void NewScene_Click(object sender, RoutedEventArgs e)
        {
            Scene.CurrentScene.UnLoad();
            Scene.CurrentScene = new Scene("Scene");
        }

        private void SaveSceneAs_Click(object sender, RoutedEventArgs e)
        {

            Microsoft.Win32.SaveFileDialog saveFileDialog = new Microsoft.Win32.SaveFileDialog();
            saveFileDialog.Filter = "Thomas Dank Scene (*.tds) |*.tds";

            string CombinedPath = System.IO.Path.Combine(Directory.GetCurrentDirectory(), "..\\Data");

            saveFileDialog.InitialDirectory = System.IO.Path.GetFullPath(CombinedPath);
            saveFileDialog.RestoreDirectory = true;
            saveFileDialog.FileName = Scene.CurrentScene.Name;
            if (saveFileDialog.ShowDialog() == true)
            {
                Scene sceneToSave = Scene.CurrentScene;
                Scene.SaveSceneAs(sceneToSave, saveFileDialog.FileName);
                sceneToSave.Name = System.IO.Path.GetFileNameWithoutExtension(saveFileDialog.FileName);
            }

        }

        private void LoadScene_Click(object sender, RoutedEventArgs e)
        {
         
            Microsoft.Win32.OpenFileDialog openFileDialog = new Microsoft.Win32.OpenFileDialog();
            openFileDialog.Filter = "Thomas Dank Scene (*.tds) |*.tds";

            string CombinedPath = System.IO.Path.Combine(Directory.GetCurrentDirectory(), "..\\Data");

            openFileDialog.InitialDirectory = System.IO.Path.GetFullPath(CombinedPath);
            openFileDialog.RestoreDirectory = true;
            if (openFileDialog.ShowDialog() == true)
            {

                Scene newScene = Scene.LoadScene(openFileDialog.FileName);
                
                Scene.CurrentScene.UnLoad();

                Scene.CurrentScene = newScene;
                                         

                //Refresh tree
                //thomasObjects.Items.Clear();

                //foreach (GameObject newItem in Scene.CurrentScene.GameObjects)
                //{
                //    if (newItem.transform.parent == null)
                //    {
                //        TreeViewItem node = new TreeViewItem { DataContext = newItem };
                //        node.MouseRightButtonUp += Node_MouseRightButtonUp;
                //        node.SetBinding(TreeViewItem.HeaderProperty, new Binding("Name"));
                //        BuildTree(newItem.transform, node);
                //        thomasObjects.Items.Add(node);
                //    }
                //}
                Scene.CurrentScene.PostLoad();

            }



           
        }

        private void scaleButton_Click(object sender, RoutedEventArgs e)
        {
            rotateButton.IsChecked = false;
            translateButton.IsChecked = false;
            scaleButton.IsChecked = true;
        }

        private void rotateButton_Click(object sender, RoutedEventArgs e)
        {
            scaleButton.IsChecked = false;
            translateButton.IsChecked = false;
            rotateButton.IsChecked = true;
        }

        private void translateButton_Click(object sender, RoutedEventArgs e)
        {
            scaleButton.IsChecked = false;
            rotateButton.IsChecked = false;
            translateButton.IsChecked = true;

        }
    }

    public static class Extensions
    {

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
