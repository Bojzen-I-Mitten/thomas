using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;
using ThomasEngine;
namespace ThomasEditor
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : System.Windows.Application
    {
        protected override void OnStartup(StartupEventArgs e)
        {
            ThomasWrapper.Start();
        }

        protected override void OnExit(ExitEventArgs e)
        {
            ThomasWrapper.Exit();
        }
    }
}
