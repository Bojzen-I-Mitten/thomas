
using System;

using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;

using System.Globalization;

using System.Windows.Input;

using ThomasEngine;

namespace ThomasEditor
{
    namespace Converters
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
    }

    namespace Inspectors
    {

        
        /// <summary>
        /// Interaction logic for GameObjectInspector.xaml
        /// </summary>
        /// 
        
        public partial class GameObjectInspector : UserControl
        {
            public static readonly DependencyProperty SelectedGameObjectProperty =
               DependencyProperty.Register(
               "SelectedGameObject",
               typeof(GameObject),
               typeof(GameObjectInspector),
               new PropertyMetadata(null));

            GameObject prevGameObject;
            //For selecting the firt element in the components list
           // int selectedComponent = 0;

            //public Collection<EditorDefinitionBase> customEditors;
            public GameObjectInspector()
            {

                InitializeComponent();
                Loaded += GameObjectInspector_Loaded;
                Unloaded += GameObjectInspector_Unloaded;
                // propertyGrid.Editors.Add(editor);

            }

            private void GameObjectInspector_Unloaded(object sender, RoutedEventArgs e)
            {
                if(prevGameObject != null)
                {
                    prevGameObject.Components.CollectionChanged -= Components_CollectionChanged;
                }
            }

            private void GameObjectInspector_Loaded(object sender, RoutedEventArgs e)
            {
                gameObjectGrid.Visibility = Visibility.Hidden;
                if (SelectedGameObject != null)
                {
                    gameObjectGrid.DataContext = SelectedGameObject;
                    gameObjectGrid.Visibility = Visibility.Visible;
                    //Expand();
                    //CreatePropertyGrids();
                    SelectedGameObject.Components.CollectionChanged += Components_CollectionChanged;
                    prevGameObject = SelectedGameObject;
                    RenderComponent rc = SelectedGameObject.GetComponent<RenderComponent>();
                    if (rc && rc.material != null)
                    {
                        Binding materialBinding = new Binding("material");
                        materialBinding.Source = rc;
                        MaterialEditor.SetBinding(MaterialInspector.MaterialProperty, materialBinding);
                    }
                }
            }

            private void Components_CollectionChanged(object sender, System.Collections.Specialized.NotifyCollectionChangedEventArgs e)
            {
                this.Dispatcher.Invoke((Action)(() => {
                    if (SelectedGameObject != null)
                    {
                        RenderComponent rc = SelectedGameObject.GetComponent<RenderComponent>();
                        if (rc != null)
                        {
                            Binding materialBinding = new Binding("material");
                            materialBinding.Source = rc;
                            MaterialEditor.SetBinding(MaterialInspector.MaterialProperty, materialBinding);
                        }
                        else
                        {
                            BindingOperations.ClearBinding(MaterialEditor, MaterialInspector.MaterialProperty);
                        }
                    }
                }));
                
              
                    
            }

            public GameObject SelectedGameObject
            {
                get
                {
                    return (GameObject)GetValue(SelectedGameObjectProperty);
                }
                set
                {
                    SetValue(SelectedGameObjectProperty, value);               
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
                addComponentsListPopup.IsOpen = true;
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
                addComponentsListPopup.IsOpen = true;
                AddComponentsFilter.Focus();
                addComponentList.ItemsSource = Component.GetAllAddableComponentTypes();
                CollectionViewSource.GetDefaultView(addComponentList.ItemsSource).Filter = ComponentsFilter;
                //selectedComponent = 0;
                //addComponentList.SelectedIndex = selectedComponent;
            }

            private void AddComponentsFilter_TextChanged(object sender, TextChangedEventArgs e)
            {

                CollectionViewSource.GetDefaultView(addComponentList.ItemsSource).Refresh();
                //selectedComponent = 0;
                //addComponentList.SelectedItem = addComponentList.Items[selectedComponent];
                //addComponentList.SelectedIndex = selectedComponent;
            }



            private void AddComponentList_SelectionChanged(object sender, SelectionChangedEventArgs e)
            {
                if (addComponentList.SelectedItem != null && Mouse.LeftButton == MouseButtonState.Pressed && addComponentList.IsMouseOver)
                {
                    lock (SelectedGameObject)
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
                if (component.GetType() != typeof(Transform))
                    component.Destroy();
            }

            //Add so that element 0 is selected from the start.
            private void AddComponentList_KeyUp(object sender, KeyEventArgs e)
            {
                //var list = addComponentList.Items;
                //switch (e.Key)
                //{
                //    case Key.Down:
                //        if (selectedComponent >= list.Count - 1) selectedComponent = 0;
                //        else selectedComponent++;
                //        addComponentList.SelectedIndex = selectedComponent;
                //        break;

                //    case Key.Up:
                //        if (selectedComponent == 0) selectedComponent = list.Count - 1;
                //        else selectedComponent--;
                //        addComponentList.SelectedIndex = selectedComponent;
                //        break;
                //    case Key.Enter:
                //        Type component = addComponentList.SelectedItem as Type;
                //        var method = typeof(GameObject).GetMethod("AddComponent").MakeGenericMethod(component);
                //        method.Invoke(SelectedGameObject, null);
                //        break;
                //}
            }

            private void AddComponentsList_MLBUp(object sender, MouseButtonEventArgs e)
            {
                var n = sender.GetType().GetCustomAttributesData();
                if (addComponentList.SelectedItem != null)
                {
                    lock (SelectedGameObject)
                    {
                        Type component = addComponentList.SelectedItem as Type;
                        var method = typeof(GameObject).GetMethod("AddComponent").MakeGenericMethod(component);
                        method.Invoke(SelectedGameObject, null);
                        addComponentsListPopup.IsOpen = false;
                    }
                }
            }

            private void AddComponentsListContainer_LostFocus(object sender, RoutedEventArgs e)
            {
                addComponentsListPopup.IsOpen = false;
            }
        }
    }
}
