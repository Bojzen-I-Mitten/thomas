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
        private static Key DeleteGameObject = Key.Delete;
        private static Key openOptionsMenu = Key.O;
        private static Key play = Key.F5;
        private static Key addComponent = Key.A;
        private static Key save = Key.S;


        public static Key GetOpenOptionsMenuKey() { return openOptionsMenu; }
        public static void SetOpenOptionsMenuKey(Key set) { openOptionsMenu = set; }

        public static Key GetAddNewEmptyObjectKey() { return addNewEmptyObject; }
        public static void SetAddNewEmptyObjectKey(Key set) { addNewEmptyObject = set; }

        public static Key GetDeleteObjectKey() { return DeleteGameObject; }
        public static void SetDeleteObjectKey(Key set) { DeleteGameObject = set; }

        public static Key GetPlayKey() { return play; }
        public static void SetPlayKey(Key set) { play = set; }

        public static Key GetAddComponentKey() { return addComponent; }
        public static void SetAddComponentKey(Key set) { addComponent = set; }

        public static Key GetSaveKey() { return save; }
        public static void SetSaveKey(Key set) { save = set; }


        //Include xmlns:commands="clr-namespace:ThomasEditor.Commands" in the file you want access to the custome commands
        //Check MainWindow.xaml for an example on how to add.

        //This is a custom command object.
        public static readonly RoutedUICommand NewGameObjectCmd = new RoutedUICommand
            (
                "Create Empty",     //The text that will show on the item that the command is bound to.
                "NewGameObject",       //Name of the command
                typeof(CustomCommands), //Owner of command
                new InputGestureCollection()
                {
                    new KeyGesture(             //Specify what keys need to be pressed.
                        addNewEmptyObject,      //The key
                        ModifierKeys.Control)   //Ctrl
                }
            );

        public static readonly RoutedUICommand DeleteGameObjectCmd = new RoutedUICommand
            (
                "Delete Game Object",     //The text that will show on the item that the command is bound to.
                "DeleteGameObject",       //Name of the command
                typeof(CustomCommands), //Owner of command
                new InputGestureCollection()
                {
                    new KeyGesture(             //Specify what keys need to be pressed.
                        DeleteGameObject)
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

        public static readonly RoutedUICommand AddComponent = new RoutedUICommand
            (
                "Add Component",
                "AddComponent",
                typeof(CustomCommands),
                new InputGestureCollection()
                {
                    new KeyGesture(addComponent, ModifierKeys.Control)
                }
            );


        public static readonly RoutedUICommand Save = new RoutedUICommand
            (
                "Save Scene",
                "Save",
                typeof(CustomCommands),
                new InputGestureCollection()
                {
                    new KeyGesture(save, ModifierKeys.Control)
                }
            );

        public static RoutedUICommand GetNewEmptyObject { get { return NewGameObjectCmd; } }
        public static RoutedUICommand GetDeleteGameObject { get { return DeleteGameObjectCmd; } }
        public static RoutedUICommand GetOpenOptionsWindow { get { return OpenOptionsWindow; } }
        public static RoutedUICommand GetPlay { get { return Play; } }
        public static RoutedUICommand GetAddCompenent { get { return AddComponent; } }
        public static RoutedUICommand GetSave { get { return Save; } }
    }
}
