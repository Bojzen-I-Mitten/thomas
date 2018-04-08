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
using System.IO;
namespace ThomasEditor
{
    /// <summary>
    /// Interaction logic for AssetBrowser.xaml
    /// </summary>
    
    public partial class AssetBrowser : UserControl
    {


        bool _isDragging = false;
        List<object> items;
        public static Dictionary<ThomasEditor.Resources.AssetTypes, BitmapImage> assetImages = new Dictionary<ThomasEditor.Resources.AssetTypes, BitmapImage>();

        FileSystemWatcher watcher;

        public AssetBrowser()
        {
            InitializeComponent();
            LoadAssetImages();
            items = CreateTree("..\\Data");
            fileTree.ItemsSource = items;

            watcher = new FileSystemWatcher("..\\Data");
            watcher.IncludeSubdirectories = true;
            
            //watcher.Created += Watcher_Created;
            //watcher.Deleted += Watcher_Deleted;
            //watcher.EnableRaisingEvents = true;
        }




        private void LoadAssetImages()
        {
            
            assetImages[ThomasEditor.Resources.AssetTypes.UNKNOWN] = new BitmapImage(new Uri("pack://application:,,/icons/assets/unknown.png"));
            assetImages[ThomasEditor.Resources.AssetTypes.SCENE] = new BitmapImage(new Uri("pack://application:,,/icons/assets/scene.png"));
            assetImages[ThomasEditor.Resources.AssetTypes.SHADER] = new BitmapImage(new Uri("pack://application:,,/icons/assets/shader.png"));
            assetImages[ThomasEditor.Resources.AssetTypes.AUDIO_CLIP] = new BitmapImage(new Uri("pack://application:,,/icons/assets/audio.png"));
            assetImages[ThomasEditor.Resources.AssetTypes.MODEL] = new BitmapImage(new Uri("pack://application:,,/icons/assets/model.png"));



        }


        private List<object> CreateTree(String directory)
        {
            List<object> nodes = new List<object>();
            String[] directories = Directory.GetDirectories(directory);
            String[] files = Directory.GetFiles(directory);
            foreach(String dir in directories)
            {
                String dirName = new DirectoryInfo(dir).Name;

                StackPanel stack = new StackPanel();
                stack.Orientation = Orientation.Horizontal;
                stack.Height = 15;
                Image image = new Image();
                image.Source = new BitmapImage(new Uri("pack://application:,,/icons/assets/dir.png"));
                TextBlock lbl = new TextBlock { Text = dirName };
                stack.Children.Add(image);
                stack.Children.Add(lbl);

                TreeViewItem item = new TreeViewItem { Header = stack };
                foreach (object i in CreateTree(dir))
                    item.Items.Add(i);

                nodes.Add(item);
            }
            foreach(String file in files)
            {
                String fileName = Path.GetFileNameWithoutExtension(file);
                ThomasEditor.Resources.AssetTypes assetType = ThomasEditor.Resources.GetResourceAssetType(file);
                if (assetType == ThomasEditor.Resources.AssetTypes.UNKNOWN)
                    continue;
                StackPanel stack = new StackPanel();
                stack.Orientation = Orientation.Horizontal;
                stack.Height = 15;
                
                Image image = new Image { Source = assetImages[assetType] };
                
                TextBlock lbl = new TextBlock { Text = fileName };
                stack.Children.Add(image);
                stack.Children.Add(lbl);
                stack.MouseDown += Asset_MouseDown;
                stack.DataContext = file;

                nodes.Add(new TreeViewItem { Header = stack, DataContext = ThomasEditor.Resources.Load(file)});
            }
            return nodes;
        }

        private void Asset_MouseDown(object sender, MouseButtonEventArgs e)
        {
            StackPanel stack = sender as StackPanel;
            String file = stack.DataContext as String;
            ThomasEditor.Resources.AssetTypes assetType = ThomasEditor.Resources.GetResourceAssetType(file);
            if(e.ClickCount == 2)
            {

                if(assetType == ThomasEditor.Resources.AssetTypes.SCENE)
                {
                    SplashScreen splash = new SplashScreen("splash.png");
                    splash.Show(false, true);
                    Scene.CurrentScene.UnLoad();
                    Scene.CurrentScene = Scene.LoadScene(file);
                    splash.Close(TimeSpan.FromSeconds(0.2));

                }else if(assetType == ThomasEditor.Resources.AssetTypes.SHADER)
                {
                    System.Diagnostics.Process.Start(file);
                }
            }
        }



      
        private void AssetBrowser_MouseMove(object sender, MouseEventArgs e)
        {
            if (!_isDragging && e.LeftButton == MouseButtonState.Pressed && fileTree.SelectedValue != null)
            {
                _isDragging = true;
                DragDrop.DoDragDrop(fileTree, fileTree.SelectedValue,
                    DragDropEffects.Move);
               
            }
            else if (e.LeftButton != MouseButtonState.Pressed)
            {
                _isDragging = false;

            }

        }

    }


}
