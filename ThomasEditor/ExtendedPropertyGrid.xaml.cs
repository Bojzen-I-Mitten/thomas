using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading;
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
using Xceed.Wpf.Toolkit.PropertyGrid;

namespace ThomasEditor
{

    public class Vector3Converter : IValueConverter
    {
        Vector3 vector;
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            vector = (Vector3)value;
            String type = (String)parameter;
            switch (type)
            {
                case "x": return vector.x;
                case "y": return vector.y;
                case "z": return vector.z;
                default: return vector.x;
            }

        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            String type = (String)parameter;
            if (value != null)
            {
                switch (type)
                {
                    case "x": vector.x = (float)(double)value; break;
                    case "y": vector.y = (float)(double)value; break;
                    case "z": vector.z = (float)(double)value; break;
                }
            }

            return vector;
        }
    }

    public class NullResourceConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return "None (" + (value as String) + ")";
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }

    /// <summary>
    /// Interaction logic for ExtendedPropertyGrid.xaml
    /// </summary>
    public partial class ExtendedPropertyGrid : UserControl
    {
        public ExtendedPropertyGrid()
        {
            InitializeComponent();
        }

        private void PropertyGrid_Loaded(object sender, RoutedEventArgs e)
        {
            PropertyGrid grid = sender as PropertyGrid;
            grid.ExpandAllProperties();
        }

        private void ResourceEditor_Drop(object sender, DragEventArgs e)
        {

            if (e.Data.GetDataPresent(typeof(TreeViewItem)))
            {
                TreeViewItem item = e.Data.GetData(typeof(TreeViewItem)) as TreeViewItem;
                if (item.DataContext is Resource)
                {
                    Resource resource = item.DataContext as Resource;
                    Label label = sender as Label;
                    PropertyItem pi = label.DataContext as PropertyItem;
                    if (resource.GetType() == pi.PropertyType)
                    {
                        Monitor.Enter(Scene.CurrentScene.GetGameObjectsLock());
                        pi.Value = resource;

                        Monitor.Exit(Scene.CurrentScene.GetGameObjectsLock());
                    }

                }
            }
        }



        private void ResourceEditor_PreviewDragOver(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(typeof(TreeViewItem)))
            {
                TreeViewItem item = e.Data.GetData(typeof(TreeViewItem)) as TreeViewItem;
                if (item.DataContext is Resource)
                {
                    Resource resource = item.DataContext as Resource;
                    Label label = sender as Label;
                    PropertyItem pi = label.DataContext as PropertyItem;
                    if (resource.GetType() == pi.PropertyType)
                        e.Handled = true;
                }
            }

        }


        private void ResourceList_Open(object sender, RoutedEventArgs e)
        {
            if (ResourceListPopup.instance != null && ResourceListPopup.instance.IsLoaded)
            {
                ResourceListPopup.instance.Close();
            }
            Button b = sender as Button;
            PropertyItem pi = b.DataContext as PropertyItem;
            Type resourceType = pi.PropertyType;

            ResourceListPopup.instance = new ResourceListPopup(pi, resourceType);
        }
    }
}
