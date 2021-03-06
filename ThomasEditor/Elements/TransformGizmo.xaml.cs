﻿using System;

using System.Windows;
using System.Windows.Controls;

using ThomasEngine;
namespace ThomasEditor
{
    /// <summary>
    /// Interaction logic for TransformGizmo.xaml
    /// </summary>
    public partial class TransformGizmo : UserControl
    {
        public TransformGizmo()
        {
            InitializeComponent();
        }

        private void CompositionTarget_Rendering(object sender, EventArgs e)
        {
            UpdateTransformGizmo();
        }

        public void UpdateTransformGizmo()
        {
            switch (ThomasWrapper.GetEditorGizmoManipulatorOperation())
            {
                case ThomasWrapper.ManipulatorOperation.TRANSLATE:
                    translateButton_Click(null, null);
                    break;
                case ThomasWrapper.ManipulatorOperation.ROTATE:
                    rotateButton_Click(null, null);
                    break;
                case ThomasWrapper.ManipulatorOperation.SCALE:
                    scaleButton_Click(null, null);
                    break;
            }
        }

        private void scaleButton_Click(object sender, RoutedEventArgs e)
        {
            rotateButton.IsChecked = false;
            translateButton.IsChecked = false;
            scaleButton.IsChecked = true;
            ThomasWrapper.SetEditorGizmoManipulatorOperation(ThomasWrapper.ManipulatorOperation.SCALE);
        }

        private void rotateButton_Click(object sender, RoutedEventArgs e)
        {
            scaleButton.IsChecked = false;
            translateButton.IsChecked = false;
            rotateButton.IsChecked = true;
            ThomasWrapper.SetEditorGizmoManipulatorOperation(ThomasWrapper.ManipulatorOperation.ROTATE);
        }

        private void translateButton_Click(object sender, RoutedEventArgs e)
        {
            scaleButton.IsChecked = false;
            rotateButton.IsChecked = false;
            translateButton.IsChecked = true;
            ThomasWrapper.SetEditorGizmoManipulatorOperation(ThomasWrapper.ManipulatorOperation.TRANSLATE);
        }

        private void transformModeButton_Click(object sender, RoutedEventArgs e)
        {
            ThomasWrapper.ToggleEditorGizmoManipulatorMode();
        }

    }
}
