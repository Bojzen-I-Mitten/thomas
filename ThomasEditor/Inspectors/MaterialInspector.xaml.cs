using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Dynamic;
using System.Globalization;
using System.Linq;
using System.Reflection;
using System.Reflection.Emit;
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
using Xceed.Wpf.Toolkit.PropertyGrid;
namespace ThomasEditor
{
    namespace Converters
    {
        public class IsEditableMaterial : IValueConverter
        {
            public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
            {
                Material mat = value as Material;
                return (mat != Material.StandardMaterial);
            }

            public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
            {
                throw new InvalidOperationException("IsEditableMaterial can only be used OneWay.");
            }
        }
    }
    namespace Inspectors
    {
        /// <summary>
        /// Interaction logic for MaterialEditor.xaml
        /// </summary>
        /// 
        public partial class MaterialInspector : UserControl, INotifyPropertyChanged
        {
            public virtual event PropertyChangedEventHandler PropertyChanged;
            public void OnPropertyChanged(String info)
            {
                if(PropertyChanged != null)
                    PropertyChanged(this, new PropertyChangedEventArgs(info));
            }

            public Material Material
            {
                get { return (Material)GetValue(MaterialProperty); }
                set { SetValue(MaterialProperty, value); }
            }

            public DictionaryPropertyGridAdapter MaterialProperties { get; set; }

            public Shader SelectedShader
            {
                get { return Material.Shader; }
                set { Material.Shader = value; MaterialUpdated(); }
            }

            public List<Shader> AvailableShaders
            {
                get { return new List<Shader>(ThomasEditor.Resources.GetResourcesOfType(typeof(Shader)).Cast<Shader>()); }
            }

            public static readonly DependencyProperty MaterialProperty =
            DependencyProperty.Register(
            "Material",
            typeof(Material),
            typeof(MaterialInspector),
            new PropertyMetadata(null, OnMaterialPropertyChanged));

            private static void OnMaterialPropertyChanged(DependencyObject source, DependencyPropertyChangedEventArgs e)
            {
                MaterialInspector materialInspector = source as MaterialInspector;
                materialInspector.MaterialUpdated();
            }

            public MaterialInspector()
            {
                InitializeComponent();
                Loaded += MaterialInspector_Loaded;
            }

            private void MaterialUpdated()
            {
                if (Material != null)
                {
                    if(MaterialProperties != null)
                    {
                        MaterialProperties.OnPropertyChanged -= Adapter_OnPropertyChanged;
                    }
                    MaterialProperties = new DictionaryPropertyGridAdapter(Material.EditorProperties);
                    MaterialProperties.OnPropertyChanged += Adapter_OnPropertyChanged;

                    OnPropertyChanged("MaterialProperties");
                }
            }

            private void MaterialInspector_Loaded(object sender, RoutedEventArgs e)
            {
                MaterialUpdated();

            }

            DictionaryPropertyGridAdapter adapter;

            public void SetMaterial(Material material)
            {
                //dynamic employee = new BusinessObject();
                //employee["swag"] = "John";
                //employee["banan"] = "Doe";
                
            }

            private void Adapter_OnPropertyChanged()
            {
                Material.EditorProperties = MaterialProperties._dictionary as Dictionary<String, object>;
            }


        }

        
    }
   
}
