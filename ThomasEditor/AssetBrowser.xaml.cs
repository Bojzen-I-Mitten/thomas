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

        enum AssetTypes
        {
            MODEL,
            TEXTURE,
            SCENE,
            SHADER,
            MATERIAL,
            SCRIPT,
            UNKNOWN
        }

        
        
        private AssetTypes GetFileAssetType(String file)
        {
            string extension = Path.GetExtension(file).Remove(0, 1);
            switch(extension)
            {
                case "fx":
                    return AssetTypes.SHADER;
                case "tds":
                    return AssetTypes.SCENE;
                default:
                    return AssetTypes.UNKNOWN;

            }
        }

        List<object> items;
        Dictionary<AssetTypes, BitmapImage> assetImages = new Dictionary<AssetTypes, BitmapImage>();

        public AssetBrowser()
        {
            InitializeComponent();
            LoadAssetImages();
            items = CreateTree("..\\Data");
            fileTree.ItemsSource = items;
        }

        private void LoadAssetImages()
        {
            assetImages[AssetTypes.UNKNOWN] = new BitmapImage(new Uri("pack://application:,,/icons/assets/unknown.png"));
            assetImages[AssetTypes.SCENE] = new BitmapImage(new Uri("pack://application:,,/icons/assets/scene.png"));
            assetImages[AssetTypes.SHADER] = new BitmapImage(new Uri("pack://application:,,/icons/assets/shader.png"));



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
                AssetTypes assetType = GetFileAssetType(file);
                if (assetType == AssetTypes.UNKNOWN)
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
                nodes.Add(new TreeViewItem { Header = stack});
            }
            return nodes;
        }

        private void Asset_MouseDown(object sender, MouseButtonEventArgs e)
        {
            StackPanel stack = sender as StackPanel;
            String file = stack.DataContext as String;
            AssetTypes assetType = GetFileAssetType(file);
            if(e.ClickCount == 2)
            {

                if(assetType == AssetTypes.SCENE)
                {
                    SplashScreen splash = new SplashScreen("splash.png");
                    splash.Show(false, true);
                    Scene.CurrentScene.UnLoad();
                    Scene.CurrentScene = Scene.LoadScene(file);
                    splash.Close(TimeSpan.FromSeconds(0.2));

                }else if(assetType == AssetTypes.SHADER)
                {
                    System.Diagnostics.Process.Start(file);
                }
            }
        }
    }


}
