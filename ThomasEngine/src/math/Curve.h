#pragma once

using namespace System::Collections::Generic;
using namespace System::Runtime::Serialization;
using namespace System;
using namespace System::Windows;

namespace ThomasEditor
{
	public ref class Curve
	{
	public:
		List<Point>^ points = gcnew List<Point>();
		bool expandedInPropertyGrid = false;

	private:
		double Interpolate(int indexLP, double x)
		{
			double p0x = points[indexLP].X;
			return (x - p0x) / (points[indexLP + 3].X - p0x);
		}

		double BezierFunction(int indexLP, double t)
		{
			double omt = (1 - t);
			double tt = t * t;
			double l0 = omt * omt * omt;
			double l1 = 3 * t * omt * omt;
			double l2 = 3 * tt * omt;
			double l3 = tt * t;
			return points[indexLP].Y * l0 + points[indexLP + 1].Y * l1 + points[indexLP + 2].Y * l2 + points[indexLP + 3].Y * l3;
		}

		int GetLinePointIndexToLeft(double x)
		{
			int currentMaxPos = 0;
			for (int i = 0; i < points->Count; i += 3)
			{
				int newMaxPos = i;

				if (points[newMaxPos].X >= x)
				{
					return currentMaxPos;
				}
				else
					currentMaxPos = newMaxPos;

			}
			return 0;
		}

	public:
		double GetYFromX(double x)
		{
			if (points->Count == 0)
				return 1;
			if (x < points[0].X)
				return points[0].Y;
			else if (x > points[points->Count - 1].X)
				return points[points->Count - 1].Y;
			else
			{
				int lp = GetLinePointIndexToLeft(x);
				return BezierFunction(lp, Interpolate(lp, x));
			}


		}

		double ParseFunction(String^ func, double x)
		{
			
			Dictionary<char, unsigned>^ operators = gcnew Dictionary<char, unsigned>();
			operators->Add('^', 1);
			operators->Add('*', 10);
			operators->Add('/', 11);
			operators->Add('+', 100);
			operators->Add('-', 101);
			operators->Add('(', 1000);
			operators->Add(')', 1001);
			List<double>^ numberStack = gcnew List<double>();
			List<unsigned>^ operatorStack = gcnew List<unsigned>();
			
			String^ delimStr = " ";
			array<Char>^ delim = delimStr->ToCharArray();
			func = String::Join("", func->Split(delim, StringSplitOptions::RemoveEmptyEntries));

			
			
			if (func[0] != 'x' && !Char::IsNumber(func[0]) && func[0] != '(' && func[0] != '-')
				return 0.0;

			double numberAccumulator = 0;
			unsigned incrementer = 1;

			for (int i = 0; i < func->Length; ++i)
			{
				char c = func[i];

				if (Char::IsNumber(c))
				{
					numberAccumulator += incrementer * (double)(c - '0');
					incrementer *= 10;
				}
				else if (c == 'x')
				{
					numberStack->Add(x);
				}
				else
				{
					if (numberAccumulator != 0)
					{
						numberStack->Add(numberAccumulator);
						incrementer = 1;
						numberAccumulator = 0;
					}
					//for (Dictionary<char, unsigned>^ o : operators)
					for each(KeyValuePair<char, unsigned> o in operators)
					{
						if (o.Key == c)
						{
							if (operatorStack->Count == 0)
							{
								operatorStack->Add(o.Value);
							}
							else
							{
								double test = 0.0;
								while (operatorStack[operatorStack->Count - 1] < o.Value * 2)
								{
									switch (operatorStack[operatorStack->Count - 1])
									{
									case 1:
										test = Math::Pow(numberStack[numberStack->Count - 2], numberStack[numberStack->Count - 1]);
										numberStack[numberStack->Count - 2] = test;
										break;
									case 10:
										numberStack[numberStack->Count - 2] = numberStack[numberStack->Count - 2] * numberStack[numberStack->Count - 1];
										break;
									case 11:
										numberStack[numberStack->Count - 2] = numberStack[numberStack->Count - 2] / numberStack[numberStack->Count - 1];
										break;
									case 100:
										numberStack[numberStack->Count - 2] = numberStack[numberStack->Count - 2] + numberStack[numberStack->Count - 1];
										break;
									case 101:
										numberStack[numberStack->Count - 2] = numberStack[numberStack->Count - 2] - numberStack[numberStack->Count - 1];
										break;
										/*case 1000:
										numberStack->Add(ParseFunction(func.Substring(i, func.Length - i), x, min, max));
										break;
										case 1001:

										break;*/
									default:
										break;
									}
									numberStack->RemoveAt(numberStack->Count - 1);
									operatorStack->RemoveAt(operatorStack->Count - 1);
									if (operatorStack->Count == 0)
									{
										break;
									}
								}
								operatorStack->Add(o.Value);
							}
							break;
						}
					}
				}
			}
			if (operatorStack->Count != 0)
			{
				numberStack->Add(numberAccumulator);

				double test = 0.0;
				for (int oi = operatorStack->Count - 1; oi > -1; --oi)
				{
					switch (operatorStack[oi])
					{
					case 1:
						test = Math::Pow(numberStack[numberStack->Count - 2], numberStack[numberStack->Count - 1]);
						numberStack[numberStack->Count - 2] = test;
						break;
					case 10:
						numberStack[numberStack->Count - 2] = numberStack[numberStack->Count - 2] * numberStack[numberStack->Count - 1];
						break;
					case 11:
						numberStack[numberStack->Count - 2] = numberStack[numberStack->Count - 2] / numberStack[numberStack->Count - 1];
						break;
					case 100:
						numberStack[numberStack->Count - 2] = numberStack[numberStack->Count - 2] + numberStack[numberStack->Count - 1];
						break;
					case 101:
						numberStack[numberStack->Count - 2] = numberStack[numberStack->Count - 2] - numberStack[numberStack->Count - 1];
						break;
					default:
						break;
					}
					numberStack->RemoveAt(numberStack->Count - 1);
					operatorStack->RemoveAt(operatorStack->Count - 1);
				}
			}
			return numberStack[0];
		}
		//Driver for approximate function
		List<Point>^ Approximate(String^ func, double xMin, double xMax, int recursions)
		{
			//Create start & end points, pass along to private approx
			
			//remove points between xMin and xMax
			for (int i = 0; i < points->Count; ++++++i)
			{
				if (points[i].X >= xMin && points[i].X <= xMax)
				{

				}
			}

			//add LP for start
			//add CP for start

			//add LP for end
			//add CP for end
		}
	private:
		List<Point>^ ApproximatePr(String^ func, double xMin, double xMax, int recursions)
		{
			if (recursions == 0)
				return;
			double h = (xMax - xMin) / 1000;
			double yMin = ParseFunction(func, xMin);
			double yMinh = ParseFunction(func, xMin + h);
			double yMax = ParseFunction(func, xMax);
			double yMaxh = ParseFunction(func, xMax - h);
			double mindx = (yMinh - yMin) / ((xMin + h) - xMin);
			double maxdx = (yMaxh - yMax) / ((xMax - h) - xMax);

			//add middle point, update existing start/end points 

			int recuCopy = recursions - 1;
			Approximate(func, xMin, xMax / 2, recuCopy);
			Approximate(func, xMax / 2, xMax, recuCopy);
		}
	};
}

