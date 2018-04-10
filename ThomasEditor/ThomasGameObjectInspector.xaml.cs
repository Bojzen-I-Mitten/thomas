
using System;
using System.Linq;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;

using System.Globalization;
using System.Windows.Threading;
using System.Collections.ObjectModel;
using System.Windows.Input;
using Xceed.Wpf.Toolkit.PropertyGrid;
using Xceed.Wpf.Toolkit.PropertyGrid.Editors;
using System.Threading;
using System.Collections.Generic;

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
            if(value != null)
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

    public class ComponentToNameConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return value.GetType().Name;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }

    /// <summary>
    /// Interaction logic for ThomasGameObjectInspector.xaml
    /// </summary>
    public partial class ThomasGameObjectInspector : UserControl
    {
        private ResourceListPopup resourceListPopup;
        private GameObject _gameObject;
        //public Collection<EditorDefinitionBase> customEditors;
        public ThomasGameObjectInspector()
        {

            InitializeComponent();
            // propertyGrid.Editors.Add(editor);

        }
        public GameObject SelectedGameObject
        {
            get
            {
                return _gameObject;
            }
            set
            {
                _gameObject = value;
                gameObjectGrid.DataContext = null;
                gameObjectGrid.Visibility = Visibility.Hidden;
                if(_gameObject != null)
                {
                    gameObjectGrid.DataContext = _gameObject;
                    gameObjectGrid.Visibility = Visibility.Visible;
                    //Expand();
                    //CreatePropertyGrids();
                }
                    
            }
        }

        private void PropertyGrid_Loaded(object sender, RoutedEventArgs e)
        {
            PropertyGrid grid = sender as PropertyGrid;
            grid.ExpandAllProperties();
        }

        private bool ComponentsFilter(object item)
        {
            if (String.IsNullOrEmpty(AddComponentsFilter.Text))
                return true;
            else
                return ((item as Type).Name.IndexOf(AddComponentsFilter.Text, StringComparison.OrdinalIgnoreCase) >= 0);
        }

        private void AddComponentButton_Click(object sender, RoutedEventArgs e)
        {
            addComponentList.SelectedItem = null;
            addComponentsListContainer.Visibility = Visibility.Visible;
            AddComponentsFilter.Focus();
            addComponentList.ItemsSource = Component.GetAllAddableComponentTypes();
            CollectionViewSource.GetDefaultView(addComponentList.ItemsSource).Filter = ComponentsFilter;
        }

        private void AddComponent_CanExecute(object sender, CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = true;
        }

        private void AddComponentsListContainer_LostFocus(object sender, RoutedEventArgs e)
        {
            DockPanel panel = sender as DockPanel;
            panel.Visibility = Visibility.Hidden;
        }

        private void AddComponentsFilter_TextChanged(object sender, TextChangedEventArgs e)
        {
            CollectionViewSource.GetDefaultView(addComponentList.ItemsSource).Refresh();
        }



        private void AddComponentList_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if(addComponentList.SelectedItem != null)
            {
                lock(SelectedGameObject)
                {
                    Type component = addComponentList.SelectedItem as Type;
                    var method = typeof(GameObject).GetMethod("AddComponent").MakeGenericMethod(component);
                    method.Invoke(SelectedGameObject, null);
                }

            }
               

        }

        private void RemoveComponentButton_Click(object sender, RoutedEventArgs e)
        {
            Button button = (Button)sender;
            Component component = (Component)button.DataContext;
            if(component.GetType() != typeof(Transform))
                component.Destroy();
        }

        private void ResourceEditor_Drop(object sender, DragEventArgs e)
        {

            if(e.Data.GetDataPresent(typeof(TreeViewItem)))
            {
                TreeViewItem item = e.Data.GetData(typeof(TreeViewItem)) as TreeViewItem;
                if(item.DataContext is Resource)
                {
                    Resource resource = item.DataContext as Resource;
                    Label label = sender as Label;
                    PropertyItem pi = label.DataContext as PropertyItem;
                    if(resource.GetType() == pi.PropertyType)
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
            if (resourceListPopup != null && resourceListPopup.IsLoaded)
            {
                resourceListPopup.Close();
            }
            Button b = sender as Button;
            PropertyItem pi = b.DataContext as PropertyItem;
            Type resourceType = pi.PropertyType;

            resourceListPopup = new ResourceListPopup(pi, resourceType);
        }
    }

}
