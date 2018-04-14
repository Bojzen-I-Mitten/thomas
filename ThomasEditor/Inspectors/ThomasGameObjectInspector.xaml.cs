﻿
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
        //For selecting the firt element in the components list
        int selectedComponent = 0;

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

        private void AddComponent(object sender, ExecutedRoutedEventArgs e)
        {
            addComponentList.SelectedItem = null;
            addComponentsListContainer.Visibility = Visibility.Visible;
            AddComponentsFilter.Focus();
            addComponentList.ItemsSource = Component.GetAllAddableComponentTypes();
            CollectionViewSource.GetDefaultView(addComponentList.ItemsSource).Filter = ComponentsFilter;
            selectedComponent = 0;
            addComponentList.SelectedIndex = selectedComponent;
        }

        private void AddComponentsListContainer_LostFocus(object sender, RoutedEventArgs e)
        {
            DockPanel panel = sender as DockPanel;
            panel.Visibility = Visibility.Hidden;
        }

        
        private void AddComponentsFilter_TextChanged(object sender, TextChangedEventArgs e)
        {

            CollectionViewSource.GetDefaultView(addComponentList.ItemsSource).Refresh();
            selectedComponent = 0;
            addComponentList.SelectedIndex = selectedComponent;
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

        //Add so that element 0 is selected from the start.
        private void AddComponentList_KeyUp(object sender, KeyEventArgs e)
        {
            var list = addComponentList.Items;
            switch (e.Key)
            {
                case Key.Down:
                    if (selectedComponent >= list.Count - 1) selectedComponent = 0;
                    else selectedComponent++;
                    addComponentList.SelectedIndex = selectedComponent;
                    break;

                case Key.Up:
                    if (selectedComponent == 0) selectedComponent = list.Count-1;
                    else selectedComponent--;
                    addComponentList.SelectedIndex = selectedComponent;
                    break;
                case Key.Enter:
                    Type component = addComponentList.SelectedItem as Type;
                    var method = typeof(GameObject).GetMethod("AddComponent").MakeGenericMethod(component);
                    method.Invoke(SelectedGameObject, null);
                    break;
            }

            
        }
    }

}
