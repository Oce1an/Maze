using System;

class Program
{
    static void Main()
    {
        Console.WriteLine("||| Матрицы |||");
        Matrix A = new Matrix(2, 3, 5, 7);
        Matrix B = new Matrix(1, 0, 0, 1);
        Matrix C = new Matrix();

        Console.WriteLine("Матрица A:\n" + A);
        Console.WriteLine("Матрица B:\n" + B);
        Console.WriteLine("Матрица C:\n" + C);

        int[,] arr = { { 4, 1 }, { 2, 3 } };
        Matrix D = new Matrix(arr);
        Console.WriteLine("Матрица D (из массива):\n" + D);

        Console.WriteLine("\n||| Индексаторы |||");
        Console.WriteLine($"A[0,1] = {A[0, 1]}");
        A[0, 1] = 99;
        Console.WriteLine("После изменения A[0,1] = 99:\n" + A);

        Console.WriteLine($"\nОпределитель A ={A.Determinant()}");
        Console.WriteLine($"Определитель C = {C.Determinant()}");

        Console.WriteLine("\n||| Математические Операторы |||");
        Matrix sum = A + B;
        Console.WriteLine("A + B:\n" + sum);
        Matrix diff = A - B;
        Console.WriteLine("\nA - B:\n" + diff);
        Matrix prod = A * B;
        Console.WriteLine("\nA * B:\n" + prod);
        Matrix scalarMul = A * 3;
        Console.WriteLine("\nA * 3:\n" + scalarMul);
        Matrix scalarDiv = A / 2;
        Console.WriteLine("\nA / 2:\n" + scalarDiv);

        Console.WriteLine("\n||| Инкремент Декремент |||");
        Console.WriteLine("Исходная B:\n" + B);
        B++;
        Console.WriteLine("\nПосле B++:\n" + B);
        B--;
        Console.WriteLine("\nПосле B--:\n" + B);

        Console.WriteLine("\n||| Операторы Сравнения |||");
        Matrix E = new Matrix(2, 3, 5, 7);
        Console.WriteLine("Новая матрица E:\n" + E + "\n\nТекущая матрица A:\n" + A);
        Console.WriteLine($"A == E ? {A == E}");
        Console.WriteLine($"A != E ? {A != E}");

        A = new Matrix(2, 3, 5, 7);
        Console.WriteLine("\nВосстановлена A:\n" + A);
        Console.WriteLine($"A == E ? {A == E}");

        Console.WriteLine("\nТекущая матрица B:\n" + B);
        Console.WriteLine($"\ndet(A) = {A.Determinant()}, det(B) = {B.Determinant()}, det(C) = {C.Determinant()}");
        Console.WriteLine($"A < B ? {A < B}");
        Console.WriteLine($"A > B ? {A > B}");

        Console.WriteLine("\n||| Логические Операторы |||");
        Console.Write("det(A) != 0: ");
        if (A) {Console.WriteLine("true");
        }else{Console.WriteLine("false");}
        Console.Write("det(C) == 0: ");
        if (C) {Console.WriteLine("false");
        }else{Console.WriteLine("true");}


        Console.WriteLine("\n||| Явные Преобразования |||");
        int detA = (int)A;
        Console.WriteLine($"det(A) через (int)A = {detA}");

        Matrix diag = (Matrix)5;
        Console.WriteLine("Матрица, полученная из числа 5:\n" + diag);
        Console.WriteLine($"det(diag) = {diag.Determinant()}");
    }
}