using System;

public class Matrix
{
    private int[,] data = new int[2, 2];

    public Matrix() { }

    public Matrix(int a11, int a12, int a21, int a22)
    {
        data[0, 0] = a11;
        data[0, 1] = a12;
        data[1, 0] = a21;
        data[1, 1] = a22;
    }

    public Matrix(int[,] matrix)
    {
        if (matrix.GetLength(0) != 2 || matrix.GetLength(1) != 2)
            throw new ArgumentException("Массив должен быть размером 2x2");
        data[0, 0] = matrix[0, 0];
        data[0, 1] = matrix[0, 1];
        data[1, 0] = matrix[1, 0];
        data[1, 1] = matrix[1, 1];
    }

    public int this[int row, int col]
    {
        get
        {
            if (row < 0 || row > 1 || col < 0 || col > 1)
                throw new IndexOutOfRangeException("Индексы должны быть 0 или 1");
            return data[row, col];
        }
        set
        {
            if (row < 0 || row > 1 || col < 0 || col > 1)
                throw new IndexOutOfRangeException("Индексы должны быть 0 или 1");
            data[row, col] = value;
        }
    }

    public int Determinant() => data[0, 0] * data[1, 1] - data[0, 1] * data[1, 0];

    public override string ToString()
    {
        return $"[{data[0, 0]}, {data[0, 1]}]\n[{data[1, 0]}, {data[1, 1]}]";
    }

    public static Matrix operator +(Matrix a, Matrix b)
    {
        return new Matrix(
            a[0, 0] + b[0, 0], a[0, 1] + b[0, 1],
            a[1, 0] + b[1, 0], a[1, 1] + b[1, 1]
        );
    }

    public static Matrix operator -(Matrix a, Matrix b)
    {
        return new Matrix(
            a[0, 0] - b[0, 0], a[0, 1] - b[0, 1],
            a[1, 0] - b[1, 0], a[1, 1] - b[1, 1]
        );
    }

    public static Matrix operator *(Matrix a, Matrix b)
    {
        int r00 = a[0, 0] * b[0, 0] + a[0, 1] * b[1, 0];
        int r01 = a[0, 0] * b[0, 1] + a[0, 1] * b[1, 1];
        int r10 = a[1, 0] * b[0, 0] + a[1, 1] * b[1, 0];
        int r11 = a[1, 0] * b[0, 1] + a[1, 1] * b[1, 1];
        return new Matrix(r00, r01, r10, r11);
    }

    public static Matrix operator *(Matrix m, int scalar)
    {
        return new Matrix(
            m[0, 0] * scalar, m[0, 1] * scalar,
            m[1, 0] * scalar, m[1, 1] * scalar
        );
    }

    public static Matrix operator *(int scalar, Matrix m) => m * scalar;

    public static Matrix operator /(Matrix m, int scalar)
    {
        if (scalar == 0) throw new DivideByZeroException("Деление на ноль");
        return new Matrix(
            m[0, 0] / scalar, m[0, 1] / scalar,
            m[1, 0] / scalar, m[1, 1] / scalar
        );
    }

    public static Matrix operator ++(Matrix m)
    {
        m[0, 0]++; m[0, 1]++;
        m[1, 0]++; m[1, 1]++;
        return m;
    }

    public static Matrix operator --(Matrix m)
    {
        m[0, 0]--; m[0, 1]--;
        m[1, 0]--; m[1, 1]--;
        return m;
    }

    public static bool operator ==(Matrix a, Matrix b)
    {
        if (ReferenceEquals(a, b)) return true;
        if (a is null || b is null) return false;
        return a[0, 0] == b[0, 0] && a[0, 1] == b[0, 1] &&
               a[1, 0] == b[1, 0] && a[1, 1] == b[1, 1];
    }

    public static bool operator !=(Matrix a, Matrix b) => !(a == b);

    public static bool operator <(Matrix? a, Matrix? b)
    {
        if (a is null || b is null) throw new ArgumentNullException();
        return a.Determinant() < b.Determinant();
    }

    public static bool operator >(Matrix? a, Matrix? b)
    {
        if (a is null || b is null) throw new ArgumentNullException();
        return a.Determinant() > b.Determinant();
    }

    public static bool operator true(Matrix m) => m.Determinant() != 0;

    public static bool operator false(Matrix m) => m.Determinant() == 0;

    public static explicit operator int(Matrix m) => m.Determinant();

    public static explicit operator Matrix(int a) => new Matrix(a, 0, 0, a);

    public override bool Equals(object? obj)
    {
        if (obj is Matrix other)
            return this == other;
        return false;
    }

    public override int GetHashCode()
    {
        return HashCode.Combine(data[0, 0], data[0, 1], data[1, 0], data[1, 1]);
    }
}