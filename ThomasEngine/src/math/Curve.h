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

		double keklel()
		{
			return 5.0;
		}

		double ParseFunction(String^% func, double x)
		{
			Dictionary<char, unsigned>^ operators = gcnew Dictionary<char, unsigned>();
			operators->Add('(', 10);
			operators->Add(')', 31);
			operators->Add('^', 100);
			operators->Add('*', 1000);
			operators->Add('/', 1001);
			operators->Add('+', 10000);
			operators->Add('-', 10001);
			
			List<double>^ numberStack = gcnew List<double>();
			List<unsigned>^ operatorStack = gcnew List<unsigned>();
			
			String^ delimStr = " ";
			array<Char>^ delim = delimStr->ToCharArray();
			func = String::Join("", func->Split(delim, StringSplitOptions::RemoveEmptyEntries));
			
			
			if (func[0] != 'x' && !Char::IsNumber(func[0]) && func[0] != '(' && func[0] != '-' && func == "")
				return 0.0;

			double numberAccumulator = 0;
			unsigned incrementer = 1;
			bool exit = false;

			for (int i = 0; i < func->Length && !exit; ++i)
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
							if (o.Value == 10)
							{
								func = func->Substring(i + 1, func->Length - i - 1);
								numberStack->Add(ParseFunction(func, x));
								if (func != "")
									i = -1;
								else
									exit = true;
							}
							else if (o.Value == 31)
							{
								func = func->Substring(i + 1, func->Length - i - 1);
								exit = true;
							}
							else if (operatorStack->Count == 0)
							{
								operatorStack->Add(o.Value);
							}
							else
							{
								while (operatorStack[operatorStack->Count - 1] < o.Value * 2)
								{
									switch (operatorStack[operatorStack->Count - 1])
									{
									/*case 10:
										numberStack->Add(ParseFunction(func->Substring(i + 1, func->Length - i - 1), x));
										numberStack->Add(0.0);//Bogo-number
										break;*/
									/*case 31:
										func = "";
										numberStack->Add(0.0);//bogo
										break;*/
									case 100:
										numberStack[numberStack->Count - 2] = Math::Pow(numberStack[numberStack->Count - 2], numberStack[numberStack->Count - 1]);
										break;
									case 1000:
										numberStack[numberStack->Count - 2] = numberStack[numberStack->Count - 2] * numberStack[numberStack->Count - 1];
										break;
									case 1001:
										numberStack[numberStack->Count - 2] = numberStack[numberStack->Count - 2] / numberStack[numberStack->Count - 1];
										break;
									case 10000:
										numberStack[numberStack->Count - 2] = numberStack[numberStack->Count - 2] + numberStack[numberStack->Count - 1];
										break;
									case 10001:
										numberStack[numberStack->Count - 2] = numberStack[numberStack->Count - 2] - numberStack[numberStack->Count - 1];
										break;
									
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
				if (!exit)
					numberStack->Add(numberAccumulator);

				for (int oi = operatorStack->Count - 1; oi > -1; --oi)
				{
					switch (operatorStack[oi])
					{

					case 100:
						numberStack[numberStack->Count - 2] = Math::Pow(numberStack[numberStack->Count - 2], numberStack[numberStack->Count - 1]);
						break;
					case 1000:
						numberStack[numberStack->Count - 2] = numberStack[numberStack->Count - 2] * numberStack[numberStack->Count - 1];
						break;
					case 1001:
						numberStack[numberStack->Count - 2] = numberStack[numberStack->Count - 2] / numberStack[numberStack->Count - 1];
						break;
					case 10000:
						numberStack[numberStack->Count - 2] = numberStack[numberStack->Count - 2] + numberStack[numberStack->Count - 1];
						break;
					case 10001:
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
		List<Point>^ Approximate(String^ func, double xMin, double xMax, int partitions)
		{
			List<Point>^ output;
			//Create start & end points, pass along to private approx
			double h = (xMax - xMin) / 1000000;
			double yMin = ParseFunction(func, xMin);
			double yMinh = ParseFunction(func, xMin + h);
			double yMax = ParseFunction(func, xMax);
			double yMaxh = ParseFunction(func, xMax - h);

			//convert to Point data type
			Point startLP = Point(xMin, yMin);
			Point startCP = Point(xMin + h, yMinh);
			Point endLP = Point(xMax, yMax);
			Point endCP = Point(xMax - h, yMaxh);

			//calculate vectors for CPs
			Vector startCPvec	= Vector(startCP.X - startLP.X, startCP.Y - startLP.Y);
			Vector endCPvec		= Vector(endCP.X - endLP.X, endCP.Y - endLP.Y); //we want this vector pointing the "opposite" way of the other one
			startCPvec.Normalize();
			endCPvec.Normalize();

			//Adjust CP distance to respective LPs according to how many line points the user wants
			startCPvec *= 0.45 / (partitions + 1);
			endCPvec *= 0.45 / (partitions + 1);
			startCP = Point(startLP.X + startCPvec.X, startLP.Y + startCPvec.Y);
			endCP = Point(endLP.X + endCPvec.X, endCP.Y + endCPvec.Y);

			//add LP for start
			output->Add(startLP);
			//add CP for start
			output->Add(startCP);

			//add CP for end
			output->Add(endCP);
			//add LP for end
			output->Add(endLP);

			output->InsertRange(2, ApproximatePr(func, xMin, xMax, partitions, partitions));

			return output;
		}
	private:
		List<Point>^ ApproximatePr(String^ func, double xMin, double xMax, int recursions, int partitions)
		{
			List<Point>^ output;
			if (recursions == 0)
				return output;
			//evaluate function to find y and yh from x and x+h
			double h = (xMax - xMin) / 1000000;
			double x = (xMax + xMin) / 2;
			double xh1 = x - h;
			double y = ParseFunction(func, x);
			double yh1 = ParseFunction(func, x - h);
			
			//convert y and x to Point datatype
			Point LP = Point(x, y);
			//Calculate CP positions from func dx
			Point CP1 = Point(x - h, yh1);
			Vector CPvec = Vector(CP1.X - LP.X, CP1.Y - LP.Y);
			CPvec.Normalize();
			CPvec *= 0.45 / (partitions + 1);
			CP1 = Point(LP.X + CPvec.X, LP.Y + CPvec.Y);
			Point CP2 = Point(LP.X - CPvec.X, LP.Y - CPvec.Y);

			//Go deeper
			recursions /= 2;
			output->InsertRange(0, ApproximatePr(func, xMin, x, recursions, partitions));
			output->InsertRange(3, ApproximatePr(func, x, xMax, recursions, partitions));

			return output;
		}
	};
}

