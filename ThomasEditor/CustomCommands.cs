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
        private static Key addNewEmptyObject = Key.N;
        private static Key openOptionsMenu = Key.O;
        private static Key play = Key.F5;

        public static Key GetOpenOptionsMenuKey() { return openOptionsMenu; }
        public static void SetOpenOptionsMenuKey(Key set) { openOptionsMenu = set; }
        public static Key GetaddNewEmptyObjectKey() { return addNewEmptyObject; }
        public static void SetaddNewEmptyObjectKey(Key set) { addNewEmptyObject = set; }
        public static Key GetPlayKey() { return play; }
        public static void SetPlayKey(Key set) { play = set; }




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
                    new KeyGesture(addNewEmptyObject, ModifierKeys.Control)
                }
            );

        public static readonly RoutedUICommand OpenOptionsWindow = new RoutedUICommand
            (
                "Options",
                "OpenOptionsWindow",
                typeof(CustomCommands),
                new InputGestureCollection()
                {
                    new KeyGesture(openOptionsMenu, ModifierKeys.Control)
                }
            );

        public static readonly RoutedUICommand Play = new RoutedUICommand
            (
                "Play",
                "Play",
                typeof(CustomCommands),
                new InputGestureCollection()
                {
                    new KeyGesture(play, ModifierKeys.None)
                }
            );

        public static RoutedUICommand GetNewEmptyObject { get { return NewEmptyObject; } }
        public static RoutedUICommand GetOpenOptionsWindow { get { return OpenOptionsWindow; } }
        public static RoutedUICommand GetPlay { get { return Play; } }
    }
}
