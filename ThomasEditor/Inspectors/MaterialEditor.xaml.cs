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
    /// <summary>
    /// Interaction logic for MaterialEditor.xaml
    /// </summary>
    /// 
    public partial class MaterialEditor : UserControl
    {
        private Material currentMat;
        Dictionary<String, object> properties;
        public MaterialEditor()
        {
            InitializeComponent();
        }
        DictionaryPropertyGridAdapter adapter;

        public void SetMaterial(Material material)
        {
            //dynamic employee = new BusinessObject();
            //employee["swag"] = "John";
            //employee["banan"] = "Doe";
            currentMat = material;
            adapter = new DictionaryPropertyGridAdapter(currentMat.EditorProperties);
            adapter.OnPropertyChanged += Adapter_OnPropertyChanged;
            propertyGrid.DataContext = adapter;
        }

        private void Adapter_OnPropertyChanged()
        {   
            currentMat.EditorProperties = adapter._dictionary as Dictionary<String, object>;
        }


    }
}
