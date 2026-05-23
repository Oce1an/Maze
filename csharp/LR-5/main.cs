using System;
using System.Collections.Generic;
using System.Linq;

public enum ClientType
{
    Standard,
    VIP
}

public class Tariff
{
    public string Name { get; set; }
    public decimal PricePerUnit { get; set; }

    public Tariff(string name, decimal pricePerUnit)
    {
        Name = name;
        PricePerUnit = pricePerUnit;
    }

    public override string ToString()
    {
        return $"Тариф \"{Name}\", цена за ед.: {PricePerUnit:F2} руб.";
    }
}

public class Client
{
    public string FullName { get; set; }
    public string Phone { get; set; }
    public ClientType Type { get; set; }

    public Client(string fullName, string phone, ClientType type = ClientType.Standard)
    {
        FullName = fullName;
        Phone = phone;
        Type = type;
    }

    public override string ToString()
    {
        return $"Клиент: {FullName} ({Type}), тел.: {Phone}";
    }
}

public class Order
{
    public Client Client { get; set; }
    public Tariff Tariff { get; set; }
    public double Volume { get; set; }

    public Order(Client client, Tariff tariff, double volume)
    {
        Client = client;
        Tariff = tariff;
        Volume = volume;
    }

    public decimal GetCost()
    {
        decimal baseCost = Tariff.PricePerUnit * (decimal)Volume;
        if (Client.Type == ClientType.VIP)
            return baseCost * 0.9m;
        return baseCost;
    }

    public override string ToString()
    {
        return $"Заказ: {Client.FullName}, тариф \"{Tariff.Name}\", объём: {Volume} м³, стоимость: {GetCost():F2} руб.";
    }
}

public class TransportCompany
{
    public string CompanyName { get; set; }
    public List<Tariff> Tariffs { get; private set; }
    public List<Client> Clients { get; private set; }
    public List<Order> Orders { get; private set; }

    public TransportCompany(string companyName)
    {
        CompanyName = companyName;
        Tariffs = new List<Tariff>();
        Clients = new List<Client>();
        Orders = new List<Order>();
    }

    public void AddTariff(Tariff tariff)
    {
        if (tariff == null)
            throw new ArgumentNullException(nameof(tariff));
        Tariffs.Add(tariff);
    }

    public Client RegisterClient(string fullName, string phone, ClientType type = ClientType.Standard)
    {
        var client = new Client(fullName, phone, type);
        Clients.Add(client);
        return client;
    }

    public Order PlaceOrder(Client client, string tariffName, double volume)
    {
        if (client == null)
            throw new ArgumentNullException(nameof(client));
        if (!Clients.Contains(client))
            throw new InvalidOperationException("Клиент не зарегистрирован в системе.");

        Tariff tariff = Tariffs.FirstOrDefault(t => t.Name == tariffName);
        if (tariff == null)
            throw new ArgumentException($"Тариф \"{tariffName}\" не найден.");

        if (volume <= 0)
            throw new ArgumentException("Объём груза должен быть положительным.");

        var order = new Order(client, tariff, volume);
        Orders.Add(order);
        return order;
    }

    public decimal GetTotalCostForClient(Client client)
    {
        return Orders
            .Where(o => o.Client == client)
            .Sum(o => o.GetCost());
    }

    public decimal GetTotalCostOfAllOrders()
    {
        return Orders.Sum(o => o.GetCost());
    }
}

class Program
{
    static void Main(string[] args)
    {
        var company = new TransportCompany("Быстрая Доставка");

        company.AddTariff(new Tariff("Эконом", 150.00m));
        company.AddTariff(new Tariff("Стандарт", 250.00m));
        company.AddTariff(new Tariff("Экспресс", 400.00m));
        Console.WriteLine("~~~~~~~~~~~~Тарифы фирмы~~~~~~~~~~~~");
        foreach (var t in company.Tariffs)
            Console.WriteLine(t);

        Console.WriteLine("\n~~~~~~~~~~~~Регистрация клиентов~~~~~~~~~~~~");
        var client1 = company.RegisterClient("Иванов Иван", "+79001234567", ClientType.Standard);
        var client2 = company.RegisterClient("Петрова Анна", "+79007654321", ClientType.VIP);
        Console.WriteLine(client1);
        Console.WriteLine(client2);

        Console.WriteLine("\n~~~~~~~~~~~~Оформление заказов~~~~~~~~~~~~");
        try
        {
            var order1 = company.PlaceOrder(client1, "Эконом", 10.5);
            var order2 = company.PlaceOrder(client2, "Экспресс", 3.0);
            var order3 = company.PlaceOrder(client2, "Стандарт", 7.2);
            Console.WriteLine(order1);
            Console.WriteLine(order2);
            Console.WriteLine(order3);
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Ошибка: {ex.Message}");
        }

        Console.WriteLine("\n~~~~~~~~~~~~Сумма заказов по клиентам~~~~~~~~~~~~");
        Console.WriteLine($"Клиент {client1.FullName}: {company.GetTotalCostForClient(client1):F2} руб.");
        Console.WriteLine($"Клиент {client2.FullName}: {company.GetTotalCostForClient(client2):F2} руб.");

        Console.WriteLine("\n~~~~~~~~~~~~Итого по фирме~~~~~~~~~~~~");
        Console.WriteLine($"Общая стоимость всех заказов: {company.GetTotalCostOfAllOrders():F2} руб.");

        Console.WriteLine("\nНажмите любую клавишу для выхода...");
        Console.ReadKey();
    }
}