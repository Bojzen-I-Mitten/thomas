﻿using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Data;
using ThomasEngine;
namespace ThomasEditor.Converters
{

    public class Vector2Converter : IValueConverter
    {
        Vector2 vector;
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            vector = (Vector2)value;
            String type = (String)parameter;
            switch (type)
            {
                case "x": return vector.x;
                case "y": return vector.y;
                default: return vector.x;
            }

        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            String type = (String)parameter;
            if (value != null)
            {
                switch (type)
                {
                    case "x": vector.x = (float)(double)value; break;
                    case "y": vector.y = (float)(double)value; break;
                }
            }

            return vector;
        }
    }

    public class Vector3Converter : IValueConverter
    {
        Vector3 vector;
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            vector = (Vector3)value;
            String type = (String)parameter;
            switch (type)
            {
                case "x": return vector.x;
                case "y": return vector.y;
                case "z": return vector.z;
                default: return vector.x;
            }

        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            String type = (String)parameter;
            if (value != null)
            {
                switch (type)
                {
                    case "x": vector.x = (float)(double)value; break;
                    case "y": vector.y = (float)(double)value; break;
                    case "z": vector.z = (float)(double)value; break;
                }
            }

            return vector;
        }
    }

    public class Vector4Converter : IValueConverter
    {
        Vector4 vector;
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (value == null)
                return 0;
            vector = (Vector4)value;
            String type = (String)parameter;
            switch (type)
            {
                case "x": return vector.x;
                case "y": return vector.y;
                case "z": return vector.z;
                case "w": return vector.w;
                default: return vector.x;
            }

        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            String type = (String)parameter;
            if (value != null)
            {
                switch (type)
                {
                    case "x": vector.x = (float)(double)value; break;
                    case "y": vector.y = (float)(double)value; break;
                    case "z": vector.z = (float)(double)value; break;
                    case "w": vector.w = (float)(double)value; break;
                }
            }

            return vector;
        }
    }

    public class ColorConverter : IValueConverter
    {

        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {

            if (value == null)
                return System.Drawing.Color.Black;
            Color color = (Color)value;
            return System.Windows.Media.Color.FromScRgb(color.a, color.r, color.g, color.b);
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (value == null)
            {
                return new Color(0, 0, 0, 1);
            }
            new Color(0, 0, 0, 1);
            System.Windows.Media.Color mediaColor = (System.Windows.Media.Color)value;
            
            //System.Drawing.Color color = System.Drawing.ColorTranslator.FromHtml(mediaColor.ToString());
            //System.Drawing.Color color = System.Drawing.ColorTranslator.FromHtml(value as string);
            return new Color(mediaColor.ScR, mediaColor.ScG, mediaColor.ScB, mediaColor.ScA);

        }
    }


    public class ObjectToTypeConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return value.GetType();
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }

    public class NullResourceConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return "None (" + (value as String) + ")";
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }

    public class IsNullConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return (value == null);
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new InvalidOperationException("IsNullConverter can only be used OneWay.");
        }
    }
}
