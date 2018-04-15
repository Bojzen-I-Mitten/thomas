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

        String path = "..\\Data";
        bool _isDragging = false;
       
        public static Dictionary<ThomasEditor.Resources.AssetTypes, BitmapImage> assetImages = new Dictionary<ThomasEditor.Resources.AssetTypes, BitmapImage>();

        FileSystemWatcher watcher;

        public AssetBrowser()
        {
            InitializeComponent();
            LoadAssetImages();
            List<object> items = CreateTree(path);
            foreach(object item in items)
            {
                fileTree.Items.Add(item);
            }

            assetBrowserContextMenu.DataContext = false;
            watcher = new FileSystemWatcher(path);
            watcher.IncludeSubdirectories = true;

            watcher.Renamed += ResourceRenamed;
            watcher.Changed += ResourceChanged;
            watcher.Created += ResourceCreated;
            watcher.Deleted += ResourceDeleted;
            watcher.EnableRaisingEvents = true;

        }

        private void ResourceRenamed(object sender, RenamedEventArgs e)
        {
            Dispatcher.BeginInvoke(new Action<String, String>((String oldPath, String newPath) =>
            {
                ThomasEditor.Resources.AssetTypes oldType = ThomasEditor.Resources.GetResourceAssetType(oldPath);
                ThomasEditor.Resources.AssetTypes newType = ThomasEditor.Resources.GetResourceAssetType(newPath);

                TreeViewItem foundItem = FindNode(oldPath);
                if (foundItem != null)
                {
                    
                    if (oldType != newType)
                        return;

                    ThomasEditor.Resources.RenameResource(oldPath, newPath);

                    StackPanel stack = foundItem.Header as StackPanel;
                    stack.DataContext = newPath;

                    String visibleName = Path.GetFileNameWithoutExtension(newPath);

                    stack.Children.OfType<TextBlock>().First().Text = visibleName;
                }
                else
                {
                    if(newType == ThomasEditor.Resources.AssetTypes.SHADER)
                    {
                        Resource shader = ThomasEditor.Resources.Find(newPath);
                        if (shader != null)
                            shader.Reload();
                    }
                }

            }), e.OldFullPath, e.FullPath);
        }

        private void ResourceChanged(object sender, FileSystemEventArgs e)
        {
            Dispatcher.BeginInvoke(new Action<String>((String p) =>
            {

                TreeViewItem foundItem = FindNode(p);
                if (foundItem != null)
                {
                   if(foundItem.DataContext is Resource)
                    {
                        Resource resource = foundItem.DataContext as Resource;
                        resource.Reload();
                    }
                }

            }), e.FullPath);
        }

        private void ResourceDeleted(object sender, FileSystemEventArgs e)
        {
            Dispatcher.BeginInvoke(new Action<String>((String p) =>
            {

                TreeViewItem foundItem = FindNode(p);
                if(foundItem != null)
                {
                    if (foundItem.Parent is TreeView)
                    {
                        fileTree.Items.Remove(foundItem);
                    }
                    else if (foundItem.Parent is TreeViewItem)
                    {
                        TreeViewItem parent = foundItem.Parent as TreeViewItem;
                        parent.Items.Remove(foundItem);
                    }
                }
              
            }), e.FullPath);
           
        }

        private List<TreeViewItem> FlattenTree(ItemCollection children)
        {
            List<TreeViewItem> flatTree = new List<TreeViewItem>();
            foreach(TreeViewItem child in children)
            {
                flatTree.Add(child);
                if(child.Items.Count > 0)
                {
                    flatTree.AddRange(FlattenTree(child.Items));
                }
            }
            return flatTree;
        }

        private TreeViewItem FindNode(String nodeName)
        {
            List<TreeViewItem> flat = FlattenTree(fileTree.Items);
            foreach (TreeViewItem item in flat)
            {
                if (((item.Header as StackPanel).DataContext as String) == nodeName)
                {
                    return item;
                }
            }
            return null;
        }

        private void AddNode(String filePath)
        {
            object item = CreateItem(filePath);
            if (item == null)
                return;
            String parentDir = filePath.Remove(filePath.LastIndexOf(Path.DirectorySeparatorChar, filePath.Length-1));

            if(parentDir == path)
            {
                    fileTree.Items.Add(item);
            }
            else
            {
                TreeViewItem dir = FindNode(parentDir);
                if (dir != null)
                    dir.Items.Add(item);
              
            }
        }

        private void ResourceCreated(object sender, FileSystemEventArgs e)
        {
            Dispatcher.BeginInvoke(new Action<String>((String p) =>
            {

                AddNode(p);

            }), e.FullPath);
        }

        private void LoadAssetImages()
        {
            
            assetImages[ThomasEditor.Resources.AssetTypes.UNKNOWN] = new BitmapImage(new Uri("pack://application:,,/icons/assets/unknown.png"));
            assetImages[ThomasEditor.Resources.AssetTypes.SCENE] = new BitmapImage(new Uri("pack://application:,,/icons/assets/scene.png"));
            assetImages[ThomasEditor.Resources.AssetTypes.SHADER] = new BitmapImage(new Uri("pack://application:,,/icons/assets/shader.png"));
            assetImages[ThomasEditor.Resources.AssetTypes.AUDIO_CLIP] = new BitmapImage(new Uri("pack://application:,,/icons/assets/audio.png"));
            assetImages[ThomasEditor.Resources.AssetTypes.MODEL] = new BitmapImage(new Uri("pack://application:,,/icons/assets/model.png"));
            assetImages[ThomasEditor.Resources.AssetTypes.MATERIAL] = new BitmapImage(new Uri("pack://application:,,/icons/assets/material.png"));
        }


        private object CreateItem(string filePath)
        {
            if(Directory.Exists(filePath)) //dir
            {
                String dirName = new DirectoryInfo(filePath).Name;

                StackPanel stack = new StackPanel();
                stack.Orientation = Orientation.Horizontal;
                stack.Height = 15;
                Image image = new Image();
                image.Source = new BitmapImage(new Uri("pack://application:,,/icons/assets/dir.png"));
                TextBlock lbl = new TextBlock { Text = dirName };
                stack.Children.Add(image);
                stack.Children.Add(lbl);
                stack.DataContext = filePath;

                TreeViewItem item = new TreeViewItem { Header = stack };
                foreach (object i in CreateTree(filePath))
                    item.Items.Add(i);

                return item;
            }
            else if(File.Exists(filePath))
            {
                String fileName = Path.GetFileNameWithoutExtension(filePath);
                ThomasEditor.Resources.AssetTypes assetType = ThomasEditor.Resources.GetResourceAssetType(filePath);
                if (assetType == ThomasEditor.Resources.AssetTypes.UNKNOWN)
                    return null;
                StackPanel stack = new StackPanel();
                stack.Orientation = Orientation.Horizontal;
                stack.Height = 15;

                ImageSource source;
                if (assetType == ThomasEditor.Resources.AssetTypes.TEXTURE2D)
                    source = new BitmapImage(new Uri(Path.GetFullPath(filePath)));
                else
                    source = assetImages[assetType];

                Image image = new Image { Source = source };

                TextBlock lbl = new TextBlock { Text = fileName };
                stack.Children.Add(image);
                stack.Children.Add(lbl);
                stack.DataContext = filePath;

                return new TreeViewItem { Header = stack, DataContext = ThomasEditor.Resources.Load(filePath) };
            }
            return null;
        }

        private List<object> CreateTree(String directory)
        {
            List<object> nodes = new List<object>();
            String[] directories = Directory.GetDirectories(directory);
            String[] files = Directory.GetFiles(directory);
            foreach(String dir in directories)
            {
                object item = CreateItem(dir);
                if(item != null)
                    nodes.Add(item);
            }
            foreach(String file in files)
            {

                object item = CreateItem(file);
                if (item != null)
                    nodes.Add(item);
            }
            return nodes;
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
        bool hasSelection { get { return true; } }

        private void AssetBrowser_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            TreeViewItem treeViewItem = VisualUpwardSearch(e.OriginalSource as DependencyObject);
            if(treeViewItem == null)
            {
                TreeViewItem item = fileTree.SelectedItem as TreeViewItem;
                if (item != null)
                    item.IsSelected = false;
                return;
            }
            StackPanel stack = treeViewItem.Header as StackPanel;
            if(treeViewItem.DataContext is Resource)
            {
                Resource resource = treeViewItem.DataContext as Resource;
                if(resource is Material)
                {
                    MaterialEditor matEdit = FindResource("materialEditor") as MaterialEditor;
                    matEdit.SetMaterial(resource as Material);
                }
            }
        }

        private void AssetBrowser_PreviewMouseRightButtonDown(object sender, MouseButtonEventArgs e)
        {
            TreeViewItem treeViewItem = VisualUpwardSearch(e.OriginalSource as DependencyObject);

            if (treeViewItem != null)
            {
                treeViewItem.Focus();
                //assetBrowserContextMenu.DataContext = true;
                //e.Handled = true;
            }
            else
            {
                TreeViewItem item = fileTree.SelectedItem as TreeViewItem;
                if(item != null)
                    item.IsSelected = false;
                assetBrowserContextMenu.DataContext = false;
            }
        }

        private TreeViewItem VisualUpwardSearch(DependencyObject source)
        {
            while (source != null && !(source is TreeViewItem))
                source = VisualTreeHelper.GetParent(source);

            return source as TreeViewItem;
        }

        private void AssetBrowser_DoubleClick(object sender, MouseButtonEventArgs e)
        {
            if (fileTree.SelectedItem == null)
                return;
            TreeViewItem item = fileTree.SelectedItem as TreeViewItem;
            StackPanel stack = item.Header as StackPanel;

            String file = stack.DataContext as String;
            ThomasEditor.Resources.AssetTypes assetType = ThomasEditor.Resources.GetResourceAssetType(file);
            if (e.ClickCount == 2)
            {

                if (assetType == ThomasEditor.Resources.AssetTypes.SCENE)
                {
                    SplashScreen splash = new SplashScreen("splash.png");
                    splash.Show(false, true);
                    Scene.CurrentScene.UnLoad();
                    Scene.CurrentScene = Scene.LoadScene(file);
                    splash.Close(TimeSpan.FromSeconds(0.2));

                }
                else if (assetType == ThomasEditor.Resources.AssetTypes.SHADER)
                {
                    System.Diagnostics.Process.Start(file);
                }
                else if (assetType == ThomasEditor.Resources.AssetTypes.MATERIAL)
                {
                    MaterialEditor matEdit = FindResource("materialEditor") as MaterialEditor;
                    matEdit.SetMaterial(ThomasEditor.Resources.Load(file) as Material);
                }
            }
        }

        #region MenuItems

        private void Menu_CreateFolder(object sender, RoutedEventArgs e)
        {

        }

        private void Menu_CreateCSharpScript(object sender, RoutedEventArgs e)
        {

        }

        private void Menu_CreateShader(object sender, RoutedEventArgs e)
        {

        }

        private void Menu_CreateScene(object sender, RoutedEventArgs e)
        {

        }

        private void Menu_CreatePrefab(object sender, RoutedEventArgs e)
        {

        }

        private void Menu_CreateMaterial(object sender, RoutedEventArgs e)
        {

        }

        private void Menu_CreateCurve(object sender, RoutedEventArgs e)
        {

        }

        private void Menu_ShowInExplorer(object sender, RoutedEventArgs e)
        {

        }

        private void Menu_OpenAsset(object sender, RoutedEventArgs e)
        {

        }

        private void Menu_DeleteAsset(object sender, RoutedEventArgs e)
        {

        }

        private void Menu_ImportAsset(object sender, RoutedEventArgs e)
        {

        }

        private void Menu_Refresh(object sender, RoutedEventArgs e)
        {

        }

        private void Menu_Reimport(object sender, RoutedEventArgs e)
        {

        }

        private void Menu_ReimportAll(object sender, RoutedEventArgs e)
        {

        }

        private void Menu_OpenCSharpProject(object sender, RoutedEventArgs e)
        {

        }
        #endregion
    }


}
