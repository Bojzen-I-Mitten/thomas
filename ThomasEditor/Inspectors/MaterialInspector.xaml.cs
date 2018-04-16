using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Dynamic;
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
    namespace Inspectors
    {
        /// <summary>
        /// Interaction logic for MaterialEditor.xaml
        /// </summary>
        /// 
        public partial class MaterialInspector : UserControl
        {
            
            public Material Material
            {
                get { return (Material)GetValue(MaterialProperty); }
                set { SetValue(MaterialProperty, value); }
            }

            public static readonly DependencyProperty MaterialProperty =
            DependencyProperty.Register(
            "Material",
            typeof(Material),
            typeof(MaterialInspector),
            new PropertyMetadata(null));

            public MaterialInspector()
            {
                InitializeComponent();
                Loaded += MaterialInspector_Loaded;
            }

            private void MaterialInspector_Loaded(object sender, RoutedEventArgs e)
            {
                if (propertyGrid.DataContext != null)
                {
                    DictionaryPropertyGridAdapter prevAdapter = propertyGrid.DataContext as DictionaryPropertyGridAdapter;
                    prevAdapter.OnPropertyChanged -= Adapter_OnPropertyChanged;
                }
                adapter = new DictionaryPropertyGridAdapter(Material.EditorProperties);
                adapter.OnPropertyChanged += Adapter_OnPropertyChanged;
                propertyGrid.DataContext = adapter;
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
                Material.EditorProperties = adapter._dictionary as Dictionary<String, object>;
            }


        }

        
    }
   
}
