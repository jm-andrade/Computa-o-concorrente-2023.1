
import java.util.Arrays;

class ListaDeSeries
{
    public double[] series;

    public ListaDeSeries(int nSeries) { this.series = new double[nSeries]; }
    public double Somatorio() { return Arrays.stream(series).sum(); }
}

class CalculaSeries extends Thread
{
    private ListaDeSeries pi;
    private int numSeries;
    private int threadID;
    private int numThreads;

    public CalculaSeries (ListaDeSeries _pi, int _numSeries, int _threadID, int _numThreads)
    {
        this.pi = _pi;
        this.numSeries = _numSeries;
        this.threadID = _threadID;
        this.numThreads = _numThreads;
    }
    public void run()
    {
        for (int i=threadID; i<numSeries; i+=numThreads)
        {
            pi.series[i] = (Math.pow(-1, i)/(2*i + 1))*4;
        }
    }
}

public class Lab3
{
    public static void main(String[] args)
    {
        final int numThreads = 4;
        final int numSeries = 10000;
        Thread[] threads = new Thread[numThreads];
        ListaDeSeries pi = new ListaDeSeries(numSeries);

        for (int i = 0; i < threads.length; i++)
        {
            threads[i] = new CalculaSeries(pi, numSeries, i, numThreads);
            threads[i].start();
        }
        for (int i = 0; i < threads.length; i++)
        {
            try { threads[i].join(); }
            catch (InterruptedException e) { return; }
        }

        if (Math.PI % pi.Somatorio() <= 0.000001)
            System.out.println("Pi = "+pi.Somatorio()+"\nBoa proximação de pi!");
        else if (Math.PI % pi.Somatorio() <= 0.01)
            System.out.println("Pi = "+pi.Somatorio()+"\nAproximação razoável de pi.");
        else
            System.out.println("Pi = "+pi.Somatorio()+"\nAproximação grosseira de pi.");
    }
}
