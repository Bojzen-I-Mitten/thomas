
using System;
using System.Linq;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;

using System.Globalization;
using System.Windows.Threading;
using System.Collections.ObjectModel;
using Xceed.Wpf.Toolkit.PropertyGrid;

namespace ThomasEditor
{
    public class ConvertVector3 : IValueConverter
    {
        Vector3 vector;
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            vector = (Vector3)value;
            String type = (String)parameter;
            switch(type)
            {
                case "x":return vector.x;
                case "y": return vector.y;
                case "z": return vector.z;
                default: return vector.x;
            }
            
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            String type = (String)parameter;
            switch (type)
            {
                case "x": vector.x = (float)(double)value; break;
                case "y": vector.y = (float)(double)value; break;
                case "z": vector.z = (float)(double)value; break;
            }
            return vector;
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
        private GameObject _gameObject;
        public Collection<EditorDefinitionBase> customEditors;
        public ThomasGameObjectInspector()
        {

            InitializeComponent();

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
                gameObjectGrid.DataContext = null;
                if(_gameObject != null)
                {
                    gameObjectGrid.DataContext = _gameObject;
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

        private void PropertyGrid_Initialized(object sender, EventArgs e)
        {
            PropertyGrid grid = sender as PropertyGrid;
            EditorTemplateDefinition x = new EditorTemplateDefinition();
            x.TargetProperties.Add(typeof(Vector3));
            x.EditingTemplate = FindResource("Vector3Editor") as DataTemplate;
            grid.EditorDefinitions.Add(x);
        }
    }

}
