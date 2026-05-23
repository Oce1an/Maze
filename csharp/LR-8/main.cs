using System;
using System.Collections.Generic;
using System.Linq;

// Интерфейс стратегии расчета цены (Паттерн Strategy)
public interface IPrice
{
    double GetPrice();
}

// Стратегия: Цена без скидки
public class NoDiscount : IPrice
{
    private double basePrice;

    public NoDiscount(double price)
    {
        basePrice = price;
    }

    public double GetPrice()
    {
        return basePrice;
    }
}

// Стратегия: Цена с фиксированной скидкой в процентах
public class AbsDiscount : IPrice
{
    private double basePrice;
    private int discountPercent;

    public AbsDiscount(double price, int discount)
    {
        basePrice = price;
        discountPercent = discount;
    }

    public double GetPrice()
    {
        return basePrice * (1 - discountPercent / 100.0);
    }
}

// Класс Тариф, использующий интерфейсную ссылку (Полиморфизм)
public class Tariff
{
    public string Name { get; private set; }
    private IPrice priceStrategy; // Интерфейсная ссылка

    public Tariff(string name, IPrice strategy)
    {
        Name = name;
        priceStrategy = strategy;
    }

    // Вызов метода интерфейса через интерфейсную ссылку
    public double GetPrice()
    {
        return priceStrategy.GetPrice();
    }

    public override string ToString()
    {
        return $"Тариф '{Name}': {GetPrice():F2} руб.";
    }
}

// Класс Фирма
public class TransportFirm
{
    private List<Tariff> tariffs = new List<Tariff>();

    // Метод добавления нового тарифа
    public void AddTariff(string name, double basePrice, int discount = 0)
    {
        IPrice strategy;
        if (discount > 0)
        {
            // Используем стратегию со скидкой
            strategy = new AbsDiscount(basePrice, discount);
        }
        else
        {
            // Используем стратегию без скидки
            strategy = new NoDiscount(basePrice);
        }

        Tariff newTariff = new Tariff(name, strategy);
        tariffs.Add(newTariff);

        Console.WriteLine($"Добавлен: {newTariff} (Базовая цена: {basePrice}, Скидка: {discount}%)");
    }

    // Метод поиска тарифа с минимальной стоимостью
    public Tariff FindMinPrice()
    {
        if (tariffs.Count == 0)
            return null;

        Tariff minTariff = tariffs[0];
        foreach (var tariff in tariffs)
        {
            if (tariff.GetPrice() < minTariff.GetPrice())
            {
                minTariff = tariff;
            }
        }
        return minTariff;
    }

    // Вспомогательный метод для вывода всех тарифов
    public void PrintAllTariffs()
    {
        Console.WriteLine("\nСписок всех тарифов:");
        if (tariffs.Count == 0)
        {
            Console.WriteLine("Список пуст.");
            return;
        }
        foreach (var t in tariffs)
        {
            Console.WriteLine(t);
        }
    }
}

// Демонстрация работы
class Program
{
    static void Main(string[] args)
    {
        TransportFirm firm = new TransportFirm();

        // 1. Добавляем тарифы без скидки
        firm.AddTariff("Эконом", 500.0);
        firm.AddTariff("Стандарт", 1200.0);

        // 2. Добавляем тарифы со скидкой (Паттерн Strategy: объекту AbsDiscount передается скидка)
        firm.AddTariff("Бизнес", 2000.0, 10);    // Скидка 10%
        firm.AddTariff("Премиум", 3500.0, 25);   // Скидка 25%
        firm.AddTariff("Выходной", 800.0, 50);   // Очень большая скидка 50% -> цена станет 400

        // 3. Вывод всех тарифов
        firm.PrintAllTariffs();

        // 4. Поиск минимального тарифа (Полиморфизм: внутри FindMinPrice вызывается GetPrice() для разных стратегий)
        Tariff cheapest = firm.FindMinPrice();

        // 5. Демонстрация вызова метода интерфейса через интерфейсную ссылку
        Console.WriteLine("\n--- Демонстрация интерфейсной ссылки ---");
        // Явно создаем объект стратегии и используем его через интерфейс
        IPrice demoStrategy = new AbsDiscount(1000, 20);
        // Создаем тариф, передавая интерфейсную ссылку
        Tariff demoTariff = new Tariff("Демо", demoStrategy);
        Console.WriteLine($"Вызов через интерфейсную ссылку: {demoTariff.GetPrice():F2} руб.");

        Console.WriteLine("\n--- Результат поиска минимального тарифа ---");
        if (cheapest != null)
        {
            Console.WriteLine($"Самый дешевый тариф: {cheapest}");
        }

        Console.ReadKey();
    }
}