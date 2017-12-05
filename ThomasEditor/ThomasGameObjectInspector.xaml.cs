using Mindscape.WpfElements.PropertyEditing;
using System;
using System.Linq;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;

using Mindscape.WpfElements.WpfPropertyGrid;
using System.Globalization;
using System.Windows.Threading;
using Mindscape.WpfElements.Themes;
using System.Collections.ObjectModel;
using Mindscape.WpfElements;

public class GetPropertyComponent : IValueConverter
{
    public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
    {
        Node node = (Node)value;
        return node.Source.ToString();
    }

    public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
    {
        throw new NotImplementedException();
    }
}

public class GetPropertyName : IValueConverter
{
    public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
    {
        String node = (String)value;
        return node.Split(':').Last<String>();
    }

    public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
    {
        throw new NotImplementedException();
    }
}



namespace ThomasEditor
{

    /// <summary>
    /// Interaction logic for ThomasGameObjectInspector.xaml
    /// </summary>
    public partial class ThomasGameObjectInspector : UserControl
    {
        private PropertyGroupDescription grouper;
        private GameObject _gameObject;
        public ThomasGameObjectInspector()
        {

            InitializeComponent();


            grouper = new PropertyGroupDescription("Node", new GetPropertyComponent());

                        
            TypeEditor editor = new TypeEditor();
            editor.EditedType = typeof(Vector3);
            editor.EditorTemplate = FindResource("Vector3Editor") as DataTemplate;

           // propertyGrid.Editors.Add(editor);

        }

        public GameObject gameObject
        {
            get
            {
                return _gameObject;
            }
            set
            {
                _gameObject = value;
                propertyGrid.ItemsSource = null;
                if(_gameObject != null)
                {
                    propertyGrid.ItemsSource = _gameObject.Components;
                    gameObjectPanel.DataContext = _gameObject;
                    Expand();
                }
                    
            }
        }


        private void Expand()
        {
            PropertyGridBindingView newBind = new PropertyGridBindingView(new ObservableCollection<Node>());

            foreach (PropertyGridRow row in propertyGrid.BindingView)
            {
                foreach (PropertyGridRow child in row.Children)
                {
                    if(child.Node.Source is Component)
                        newBind.Add(child);
                }

            }
            propertyGrid.BindingView = newBind;
            propertyGrid.Refresh();
            propertyGrid.BindingView.DefaultView.GroupDescriptions.Clear();
            propertyGrid.BindingView.DefaultView.GroupDescriptions.Add(grouper);
        }
    }

}
