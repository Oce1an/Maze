using System;
using System.Collections.Generic;
using System.Linq;

public class Tariff
{
    private decimal _rate;
    private double _cargoMass;
    private string _firmName;

    public decimal Rate
    {
        get => _rate;
        set
        {
            if (value < 0) throw new ArgumentException("Ставка не может быть отрицательной.");
            _rate = value;
        }
    }

    public double CargoMass
    {
        get => _cargoMass;
        set
        {
            if (value < 0) throw new ArgumentException("Масса груза не может быть отрицательной.");
            _cargoMass = value;
        }
    }

    public string FirmName
    {
        get => _firmName;
        set
        {
            if (string.IsNullOrWhiteSpace(value)) throw new ArgumentException("Наименование фирмы не может быть пустым.");
            _firmName = value;
        }
    }

    public Tariff(decimal rate, double cargoMass, string firmName)
    {
        Rate = rate;
        CargoMass = cargoMass;
        FirmName = firmName;
    }

    public Tariff(decimal rate, double cargoMass) : this(rate, cargoMass, "Неизвестная фирма") { }

    public void IncreaseRate(decimal amount)
    {
        if (amount < 0) throw new ArgumentException("Сумма увеличения не может быть отрицательной.");
        Rate += amount;
    }

    public void DecreaseRate(decimal amount)
    {
        if (amount < 0) throw new ArgumentException("Сумма уменьшения не может быть отрицательной.");
        Rate -= amount;
    }

    public decimal CalculateRevenue() => Rate * (decimal)CargoMass;

    public static Tariff CreateDefault() => new Tariff(10.0m, 0.0, "Фирма по умолчанию");

    public override string ToString() =>
        $"Фирма: {FirmName}, Ставка: {Rate} за тонну, Масса: {CargoMass} т, Выручка: {CalculateRevenue()}";
}

public sealed class Firm
{
    private static Firm _instance;
    private List<Tariff> _tariffs;

    private Firm() => _tariffs = new List<Tariff>();

    public static Firm Instance => _instance ??= new Firm();

    public int TariffCount => _tariffs.Count;

    public void AddTariff(Tariff tariff)
    {
        if (tariff == null) throw new ArgumentNullException(nameof(tariff));
        _tariffs.Add(tariff);
    }

    public void AddTariff(decimal rate, double mass, string firmName) =>
        _tariffs.Add(new Tariff(rate, mass, firmName));

    public bool RemoveTariff(Tariff tariff) => _tariffs.Remove(tariff);

    public Tariff GetTariff(int index)
    {
        if (index < 0 || index >= _tariffs.Count) throw new IndexOutOfRangeException("Индекс вне диапазона.");
        return _tariffs[index];
    }

    public List<Tariff> GetAllTariffs() => new List<Tariff>(_tariffs);

    public decimal CalculateTotalRevenue() => _tariffs.Sum(t => t.CalculateRevenue());

    public void IncreaseAllRates(decimal amount)
    {
        foreach (var t in _tariffs) t.IncreaseRate(amount);
    }

    public void DecreaseAllRates(decimal amount)
    {
        foreach (var t in _tariffs) t.DecreaseRate(amount);
    }

    public static void ResetInstance() => _instance = null;

    public override string ToString() =>
        $"Фирма (Singleton). Всего тарифов: {TariffCount}. Общая выручка: {CalculateTotalRevenue()}";
}