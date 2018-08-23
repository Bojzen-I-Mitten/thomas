using System;

using System.Windows;
using System.Windows.Controls;
using ThomasEngine;
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
