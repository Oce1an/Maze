using System;

public abstract class ComputerTech
{
    public string Manufacturer { get; set; }
    public string Model { get; set; }
    public int Year { get; set; }
    public decimal Price { get; set; }

    public ComputerTech(string manufacturer, string model, int year, decimal price)
    {
        Manufacturer = manufacturer;
        Model = model;
        Year = year;
        Price = price;
    }

    public abstract void DisplayInfo();

    public void UpdatePrice(decimal newPrice)
    {
        Price = newPrice;
        Console.WriteLine($"Цена обновлена: {Price} руб.");
    }

    public virtual void Repair()
    {
        Console.WriteLine("Выполняется ремонт.");
    }

    public void ShowPrice()
    {
        Console.WriteLine($"Цена: {Price} руб.");
    }

    public virtual void Diagnose()
    {
        Console.WriteLine("Диагностика.");
    }
}

public class Laptop : ComputerTech
{
    public double ScreenSize { get; set; }
    public int BatteryLife { get; set; }

    public Laptop(string manufacturer, string model, int year, decimal price,
                  double screenSize, int batteryLife)
        : base(manufacturer, model, year, price)
    {
        ScreenSize = screenSize;
        BatteryLife = batteryLife;
    }

    public override void DisplayInfo()
    {
        Console.WriteLine($"{Manufacturer}, {Model}, {Year} г, " +
                          $"Экран: {ScreenSize}\", Батарея: {BatteryLife} ч, Цена: {Price} руб");
    }

    public override void Repair()
    {
        Console.WriteLine("Ремонт ноутбука.");
    }

    public void Diagnose(bool quick)
    {
        if (quick)
            Console.WriteLine("Быстрая диагностика: проверка температуры.");
        else
            Console.WriteLine("Полная диагностика: тест SSD, батареи, стресс-тест CPU.");
    }
}

public sealed class GamingLaptop : Laptop
{
    public string GraphicsCard { get; set; }

    public GamingLaptop(string manufacturer, string model, int year, decimal price,
                        double screenSize, int batteryLife, string graphicsCard)
        : base(manufacturer, model, year, price, screenSize, batteryLife)
    {
        GraphicsCard = graphicsCard;
    }

    public override void DisplayInfo()
    {
        Console.WriteLine($"{Manufacturer}, {Model}, {Year} г, " +
                          $"GPU: {GraphicsCard}, Экран: {ScreenSize}\", Батарея: {BatteryLife} ч, Цена: {Price} руб");
    }
}

public class Tablet : ComputerTech
{
    public double ScreenSize { get; set; }
    public bool StylusSupport { get; set; }

    public Tablet(string manufacturer, string model, int year, decimal price,
                  double screenSize, bool stylusSupport)
        : base(manufacturer, model, year, price)
    {
        ScreenSize = screenSize;
        StylusSupport = stylusSupport;
    }

    public override void DisplayInfo()
    {
        string stylus = StylusSupport ? "есть" : "нет";
        Console.WriteLine($"{Manufacturer}, {Model}, {Year} г, " +
                          $"Экран: {ScreenSize}\", Стилус: {stylus}, Цена: {Price} руб");
    }

    public new void ShowPrice()
    {
        Console.WriteLine("Информация о цене недоступна. Обратитесь к продавцу.");
        base.ShowPrice();
    }
}

class Program
{
    static void Main()
    {
        Console.WriteLine("\n||| Ноутбук |||");
        Laptop laptop = new Laptop("Dell", "XPS 15", 2023, 2100m, 15.6, 10);
        laptop.DisplayInfo();
        laptop.UpdatePrice(2000m);
        laptop.Repair();
        laptop.Diagnose(true);

        Console.WriteLine("\n||| Планшет |||");
        Tablet tablet = new Tablet("Apple", "iPad Pro", 2022, 1200, 12.9, true);
        tablet.DisplayInfo();
        tablet.Repair();

        Console.WriteLine("\n||| Игровой ноутбук |||");
        GamingLaptop gl = new GamingLaptop("ASUS", "ROG Zephyrus", 2024, 4000m, 16.0, 8, "RTX 4070");
        gl.DisplayInfo();
        gl.Repair();
        gl.Diagnose(false);
    }
}