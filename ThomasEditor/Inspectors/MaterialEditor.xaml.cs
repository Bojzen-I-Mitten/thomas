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
using System.Windows.Shapes;

namespace ThomasEditor
{
    /// <summary>
    /// Interaction logic for MaterialEditor.xaml
    /// </summary>
    public partial class MaterialEditor : UserControl
    {
        private Material material;
        Dictionary<String, object> properties;
        public MaterialEditor()
        {
            InitializeComponent();
        }


        public void SetMaterial(Material material)
        {
            Dictionary<String, int> d = new Dictionary<String, int>();
            d["test"] = 5;
            d["test32"] = 32;
            properties = material.EditorProperties;
            
            DictionaryPropertyGridAdapter adapter = new DictionaryPropertyGridAdapter(d);

            //propertyGrid.DataContext = adapter;
            test.SelectedObject = adapter;
        }

        private void Adapter_PropertyChanged(object sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            throw new NotImplementedException();
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            material.EditorProperties = properties;
        }
    }
}
