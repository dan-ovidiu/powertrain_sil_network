import subprocess
import csv
import io
import matplotlib.pyplot as plt

def run_network_simulation():
    print("Executing Distributed vECU Network Simulation...")
    
    # Run the compiled executable (Assuming we run python from the SIL_Simulation folder, or root)
    import os
    exe_path = '.\\network_sil.exe' if os.path.exists('.\\network_sil.exe') else '..\\network_sil.exe'
    process = subprocess.run([exe_path], capture_output=True, text=True)
    
    if process.returncode != 0:
        print("Simulation crashed!")
        return None
        
    # Read the CSV output from stdout
    reader = csv.DictReader(io.StringIO(process.stdout.strip()))
    
    steps = []
    throttles = []
    gears = []
    rpms = []
    speeds = []
    
    for row in reader:
        try:
            steps.append(int(row['Step']))
            throttles.append(float(row['Throttle']))
            gears.append(int(row['Gear']))
            rpms.append(float(row['RPM']))
            speeds.append(float(row['Speed']))
        except ValueError:
            pass
            
    return steps, throttles, gears, rpms, speeds

def main():
    steps, throttles, gears, rpms, speeds = run_network_simulation()
    
    if not steps:
        return

    # Create a 4-part plot to visualize the entire network
    fig, (ax1, ax2, ax3, ax4) = plt.subplots(4, 1, figsize=(10, 10), sharex=True)
    
    # Plot 1: The Stimulus (Driver Throttle)
    ax1.plot(steps, throttles, 'b-', linewidth=2)
    ax1.set_ylabel('Throttle (%)')
    ax1.set_title('Automated Powertrain Network Test (CI/CD)')
    ax1.grid(True)
    
    # Plot 2: Transmission vECU (Gear)
    ax2.step(steps, gears, 'g-', linewidth=2, where='post')
    ax2.set_ylabel('Gear')
    ax2.set_yticks([1, 2, 3])
    ax2.grid(True)
    
    # Plot 3: Engine vECU (RPM)
    ax3.plot(steps, rpms, 'r-', linewidth=2)
    ax3.set_ylabel('Engine RPM')
    ax3.grid(True)
    
    # Plot 4: Vehicle Speed (km/h)
    ax4.plot(steps, speeds, 'm-', linewidth=2)
    ax4.set_ylabel('Speed (km/h)')
    ax4.set_xlabel('Simulation Step (10ms)')
    ax4.grid(True)
    
    plt.tight_layout()
    plt.show()

if __name__ == "__main__":
    main()
