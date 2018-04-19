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

namespace ThomasEditor.Inspectors
{
    /// <summary>
    /// Interaction logic for Inspector.xaml
    /// </summary>
    public partial class Inspector : UserControl
    {
        private object _SelectedObject = null;
        public object SelectedObject
        {
            get { return _SelectedObject; }
            set { _SelectedObject = value; SelectedObjectType = value != null ? value.GetType() : null; }
        }

        public Type SelectedObjectType
        {
            get { return (Type)GetValue(SelectedObjectTypeProperty); }
            set { SetValue(SelectedObjectTypeProperty, value); }
        }
        public static Inspector instance;
        public Inspector()
        {
            InitializeComponent();
            instance = this;
        }

        public static readonly DependencyProperty SelectedObjectTypeProperty =
           DependencyProperty.Register(
           "SelectedObjectType",
           typeof(Type),
           typeof(Inspector),
           new PropertyMetadata(null));
    }
}
