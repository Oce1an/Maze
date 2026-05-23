using System;

class Program
{
    static void Main()
    {
        Firm firm = Firm.Instance;
        bool exit = false;

        while (!exit)
        {
            Console.WriteLine("\n--- Меню ---");
            Console.WriteLine("1. Добавить тариф");
            Console.WriteLine("2. Показать все тарифы");
            Console.WriteLine("3. Увеличить все тарифы");
            Console.WriteLine("4. Уменьшить все тарифы");
            Console.WriteLine("5. Выйти");
            Console.Write("Выберите действие: ");

            string choice = Console.ReadLine();

            try
            {
                switch (choice)
                {
                    case "1":
                        AddTariffInteractive(firm);
                        break;
                    case "2":
                        ShowAllTariffs(firm);
                        break;
                    case "3":
                        IncreaseAllRatesInteractive(firm);
                        break;
                    case "4":
                        DecreaseAllRatesInteractive(firm);
                        break;
                    case "5":
                        exit = true;
                        break;
                    default:
                        Console.WriteLine("Неверный ввод.");
                        break;
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Ошибка: {ex.Message}");
            }
        }
    }

    static void AddTariffInteractive(Firm firm)
    {
        Console.Write("Введите ставку за тонну: ");
        decimal rate = decimal.Parse(Console.ReadLine());

        Console.Write("Введите массу перевезенных грузов: ");
        double mass = double.Parse(Console.ReadLine());

        Console.Write("Введите название фирмы: ");
        string name = Console.ReadLine();

        firm.AddTariff(rate, mass, name);
        Console.WriteLine("Тариф добавлен.");
    }

    static void ShowAllTariffs(Firm firm)
    {
        var tariffs = firm.GetAllTariffs();
        if (tariffs.Count == 0)
        {
            Console.WriteLine("Нет тарифов.");
            return;
        }

        Console.WriteLine("Список тарифов:");
        for (int i = 0; i < tariffs.Count; i++)
        {
            Console.WriteLine($"{i + 1}. {tariffs[i]}");
        }
    }

    static void IncreaseAllRatesInteractive(Firm firm)
    {
        Console.Write("Введите сумму увеличения: ");
        decimal amount = decimal.Parse(Console.ReadLine());
        firm.IncreaseAllRates(amount);
        Console.WriteLine("Ставки увеличены.");
    }

    static void DecreaseAllRatesInteractive(Firm firm)
    {
        Console.Write("Введите сумму уменьшения: ");
        decimal amount = decimal.Parse(Console.ReadLine());
        firm.DecreaseAllRates(amount);
        Console.WriteLine("Ставки уменьшены.");
    }
}