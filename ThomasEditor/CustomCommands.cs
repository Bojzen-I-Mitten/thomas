using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace ThomasEditor.Commands
{
    public static class CustomCommands
    {
        //Please do this if you add new bindings
        private static Key openOptionsMenu = Key.O;
        private static Key addNewEmptyObject = Key.N;

        public static Key GetOpenOptionsMenuKey() { return openOptionsMenu; }
        public static void SetOpenOptionsMenuKey(Key set) { openOptionsMenu = set; }
        public static Key GetaddNewEmptyObjectKey() { return addNewEmptyObject; }
        public static void SetaddNewEmptyObjectKey(Key set) { addNewEmptyObject = set; }




        //Include xmlns:commands="clr-namespace:ThomasEditor.Commands" in the file you want access to the custome commands
        //Check MainWindow.xaml for an example on how to add.

        //This is a custom command object.
        public static readonly RoutedUICommand NewEmptyObject = new RoutedUICommand
            (
                "New Empty Object",   //The text that will show on the item that the command is bound to.
                "NewEmptyObject",   //Name of the command
                typeof(CustomCommands), //Owner of command
                new InputGestureCollection()
                {
                    new KeyGesture(Key.N, ModifierKeys.Control)
                }
            );

        public static readonly RoutedUICommand OpenOptionsWindow = new RoutedUICommand
            (
                "Options",
                "OpenOptionsWindow",
                typeof(CustomCommands),
                new InputGestureCollection()
                {
                    new KeyGesture(Key.O, ModifierKeys.Control)
                }
            );

        public static RoutedUICommand GetNewEmptyObject { get { return NewEmptyObject; } }
        public static RoutedUICommand GetOpenOptionsWindow { get { return OpenOptionsWindow; } }
    }
}
