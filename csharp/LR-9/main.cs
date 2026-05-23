using System;
using System.Collections.Generic;

namespace Lab9_MedPersonnel
{
    // 1. Перечисление для профиля медперсонала
    public enum ProfileType
    {
        Surgeon,
        Therapist,
        Nurse
    }

    // 2. Интерфейс для дополнительных функций

    // Делать укол
    public interface IInjectionable
    {
        void GiveInjection();
    }

    // Выписывать больничный
    public interface ISickLeaveIssuable
    {
        void IssueSickLeave();
    }

    // Делать перевязку
    public interface IBandagingable
    {
        void PerformBandaging();
    }

    // --- ПАТТЕРН "МОСТ" (Bridge) ---
    // Интерфейс метода диагностики (Implementor)
    public interface IDiagnosisMethod
    {
        void Diagnose();
    }

    // Конкретная реализация: Осмотр
    public class Examination : IDiagnosisMethod
    {
        public void Diagnose()
        {
            Console.WriteLine("   [Метод диагностики]: Визуальный осмотр пациента.");
        }
    }

    // Конкретная реализация: МРТ
    public class MRT : IDiagnosisMethod
    {
        public void Diagnose()
        {
            Console.WriteLine("   [Метод диагностики]: Проведение МРТ-сканирования.");
        }
    }

    // Конкретная реализация: Рентген
    public class XRay : IDiagnosisMethod
    {
        public void Diagnose()
        {
            Console.WriteLine("   [Метод диагностики]: Проведение рентгенографии.");
        }
    }

    // Абстрактный базовый класс (Abstraction в паттерне Мост)
    public abstract class MedicalWorkerBase
    {
        public string Name { get; set; }
        public ProfileType Profile { get; set; }

        // Ссылка на метод диагностики (Мост)
        protected IDiagnosisMethod diagnosisMethod;

        // Абстрактный метод – каждый наследник реализует по-своему
        public abstract void GetInfo();

        // Общий метод диагностики, делегирующий выполнение объекту IDiagnosisMethod
        public virtual void PerformDiagnosis()
        {
            Console.Write($"{Name} начинает диагностику: ");
            diagnosisMethod?.Diagnose();
        }

        // Установка метода диагностики (для паттерна Мост)
        public void SetDiagnosisMethod(IDiagnosisMethod method)
        {
            diagnosisMethod = method;
            Console.WriteLine($"   >> {Name} назначен метод диагностики: {method.GetType().Name}");
        }

        protected void Say(string message)
        {
            Console.WriteLine($"[{Name}]: {message}");
        }
    }

    // Конкретные классы персонала

    // Хирург: может делать уколы и перевязки
    public class Surgeon : MedicalWorkerBase, IInjectionable, IBandagingable
    {
        public override void GetInfo()
        {
            Console.WriteLine($"\n--- Информация о сотруднике ---");
            Console.WriteLine($"Имя: {Name}, Должность: Хирург");
        }

        public override void PerformDiagnosis()
        {
            Console.Write($"{Name} (хирург) оценивает состояние перед операцией: ");
            diagnosisMethod?.Diagnose();
        }

        public void GiveInjection()
        {
            Say("Делаю хирургическую инъекцию (анестезию).");
        }

        public void PerformBandaging()
        {
            Say("Накладываю стерильную повязку после операции.");
        }
    }

    // Терапевт: может выписывать больничный
    public class Therapist : MedicalWorkerBase, ISickLeaveIssuable
    {
        public override void GetInfo()
        {
            Console.WriteLine($"\n--- Информация о сотруднике ---");
            Console.WriteLine($"Имя: {Name}, Должность: Терапевт");
        }

        public override void PerformDiagnosis()
        {
            Console.Write($"{Name} (терапевт) проводит первичный прием: ");
            diagnosisMethod?.Diagnose();
        }

        public void IssueSickLeave()
        {
            Say("Выписываю больничный лист на 7 дней.");
        }
    }

    // Медсестра: может делать уколы и перевязки
    public class Nurse : MedicalWorkerBase, IInjectionable, IBandagingable
    {
        public override void GetInfo()
        {
            Console.WriteLine($"\n--- Информация о сотруднике ---");
            Console.WriteLine($"Имя: {Name}, Должность: Медсестра");
        }

        public override void PerformDiagnosis()
        {
            Console.Write($"{Name} (медсестра) выполняет назначение врача: ");
            diagnosisMethod?.Diagnose();
        }

        public void GiveInjection()
        {
            Say("Делаю внутримышечную инъекцию согласно листу назначений.");
        }

        public void PerformBandaging()
        {
            Say("Меняю повязку и обрабатываю рану.");
        }
    }

    // --- ПАТТЕРН "СТРОИТЕЛЬ" (Builder) ---

    public abstract class MedicalWorkerBuilder
    {
        protected string name = string.Empty;
        protected IDiagnosisMethod diagnosisMethod;

        public MedicalWorkerBuilder SetName(string name)
        {
            this.name = name;
            return this;
        }

        public MedicalWorkerBuilder SetDiagnosisMethod(IDiagnosisMethod method)
        {
            this.diagnosisMethod = method;
            return this;
        }

        public abstract MedicalWorkerBase Build();
    }

    public class SurgeonBuilder : MedicalWorkerBuilder
    {
        public override MedicalWorkerBase Build()
        {
            var surgeon = new Surgeon { Name = name, Profile = ProfileType.Surgeon };
            if (diagnosisMethod != null)
                surgeon.SetDiagnosisMethod(diagnosisMethod);
            return surgeon;
        }
    }

    public class TherapistBuilder : MedicalWorkerBuilder
    {
        public override MedicalWorkerBase Build()
        {
            var therapist = new Therapist { Name = name, Profile = ProfileType.Therapist };
            if (diagnosisMethod != null)
                therapist.SetDiagnosisMethod(diagnosisMethod);
            return therapist;
        }
    }

    public class NurseBuilder : MedicalWorkerBuilder
    {
        public override MedicalWorkerBase Build()
        {
            var nurse = new Nurse { Name = name, Profile = ProfileType.Nurse };
            if (diagnosisMethod != null)
                nurse.SetDiagnosisMethod(diagnosisMethod);
            return nurse;
        }
    }

    // Директор (Director) – управляет процессом создания
    public static class MedicalDirector
    {
        // Создать обычного терапевта с осмотром
        public static MedicalWorkerBase GetGeneralTherapist(string name, TherapistBuilder builder)
        {
            return builder.SetName(name)
                          .SetDiagnosisMethod(new Examination())
                          .Build();
        }

        // Создать хирурга, использующего МРТ
        public static MedicalWorkerBase GetSurgeonWithMRT(string name, SurgeonBuilder builder)
        {
            return builder.SetName(name)
                          .SetDiagnosisMethod(new MRT())
                          .Build();
        }

        // Создать медсестру, делающую рентген (по назначению)
        public static MedicalWorkerBase GetNurseWithXRay(string name, NurseBuilder builder)
        {
            return builder.SetName(name)
                          .SetDiagnosisMethod(new XRay())
                          .Build();
        }
    }

    // --- ГЛАВНАЯ ПРОГРАММА ---
    class Program
    {
        static void Main(string[] args)
        {
            // Коллекция разных объектов медперсонала
            var staff = new List<MedicalWorkerBase>();

            // Создаем строители
            var therapistBuilder = new TherapistBuilder();
            var surgeonBuilder = new SurgeonBuilder();
            var nurseBuilder = new NurseBuilder();

            // Используем Директора для создания объектов
            staff.Add(MedicalDirector.GetGeneralTherapist("Иван Петрович", therapistBuilder));
            staff.Add(MedicalDirector.GetSurgeonWithMRT("Анна Сергеевна", surgeonBuilder));
            staff.Add(MedicalDirector.GetNurseWithXRay("Ольга Николаевна", nurseBuilder));

            // Создадим еще одного сотрудника вручную, чтобы показать гибкость
            var surgeonBuilder2 = new SurgeonBuilder();
            var traumaSurgeon = surgeonBuilder2.SetName("Петр Алексеевич").SetDiagnosisMethod(new XRay()).Build();
            staff.Add(traumaSurgeon);

            Console.WriteLine("=== ОБХОД ПЕРСОНАЛА И ВЫЗОВ ВСЕХ ДОСТУПНЫХ МЕТОДОВ ===\n");

            // Обходим коллекцию и вызываем все доступные методы
            foreach (var worker in staff)
            {
                // 1. Вызов общего метода GetInfo() (полиморфно)
                worker.GetInfo();

                // 2. Вызов метода диагностики (паттерн Мост + полиморфизм)
                worker.PerformDiagnosis();

                // 3. Проверка и вызов дополнительных интерфейсов
                // Проверяем, умеет ли сотрудник делать уколы
                if (worker is IInjectionable injector)
                {
                    injector.GiveInjection();
                }

                // Проверяем, может ли выписать больничный
                if (worker is ISickLeaveIssuable sickLeaveIssuer)
                {
                    sickLeaveIssuer.IssueSickLeave();
                }

                // Проверяем, может ли сделать перевязку
                if (worker is IBandagingable bandager)
                {
                    bandager.PerformBandaging();
                }

                Console.WriteLine("---\n");
            }

            Console.ReadKey();
        }
    }
}