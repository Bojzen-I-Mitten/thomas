using System;
using ThomasEngine;
namespace ThomasEditor.utils
{

    class ScriptAssemblyManager
    {
        static string assemblyPath = "";
        public static bool CreateSolution(string path, string name)
        {
            Type type = Type.GetTypeFromProgID("VisualStudio.DTE");
            object obj = Activator.CreateInstance(type, true);
            EnvDTE.DTE dte = (EnvDTE.DTE)obj;
            try
            {
                
                dte.MainWindow.Visible = false; // optional if you want to See VS doing its thing

                string template = System.IO.Path.GetFullPath("../Data/thomasProject/MyTemplate.vstemplate");
                dte.Solution.AddFromTemplate(template, path, name);

                //create a new solution
                dte.Solution.Create(path, name + ".sln");
                var solution = dte.Solution;
                EnvDTE.Project project = solution.AddFromFile(path + "\\" + name + ".csproj");

                // create a C# class library
                System.IO.Directory.CreateDirectory(path + "\\Assets");
                project.ProjectItems.AddFromDirectory(path + "\\Assets");
                assemblyPath = path + "\\" + name + ".sln";

                // save and quit
                dte.ExecuteCommand("File.SaveAll");
                dte.Quit();
                return true;

            }
            catch(Exception e)
            {
                Debug.Log("Error creating project: " + e.Message);
                return false;
            }
        }

        public static bool OpenSolution(string path)
        {
            assemblyPath = path;
            return BuildSolution();
        }

        public static bool BuildSolution()
        {
            
            Type type = Type.GetTypeFromProgID("VisualStudio.DTE");
            object obj = Activator.CreateInstance(type, true);
            EnvDTE.DTE dte = (EnvDTE.DTE)obj;

            try
            {
                dte.MainWindow.Visible = false; // optional if you want to See VS doing its thing
                dte.Solution.Open(assemblyPath);
                var solution = dte.Solution;
                solution.SolutionBuild.Build(true);
                dte.Quit();
                return true;
            }catch(Exception e)
            {
                Debug.Log("Failed to open/build project: " + e.Message);
                dte.Quit();
                return false;
            }
        }

        public static void AddScript(string script)
        {
            Type type = Type.GetTypeFromProgID("VisualStudio.DTE");
            object obj = Activator.CreateInstance(type, true);
            EnvDTE.DTE dte = (EnvDTE.DTE)obj;

            try
            {
                dte.MainWindow.Visible = false; // optional if you want to See VS doing its thing
                dte.Solution.Open(assemblyPath);
                var solution = dte.Solution;
                solution.Projects.Item(1).ProjectItems.AddFromFile(script);
                dte.ExecuteCommand("File.SaveAll");
                solution.SolutionBuild.Build(true);
            }catch(Exception e)
            {
                Debug.Log("Failed to add file to solution: " + e.Message);
            }

            dte.Quit();
        }

    }
}
