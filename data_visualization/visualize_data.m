close all;
% Read the CSV file
data = readmatrix('kp2.txt');
% Extract columns
time = data(:,1); % First column is time/index
pitch = data(:,2); % Second column is pitch
pwm = data(:,3); % Third column is PWM
% Define time step (in seconds)
dt = 1; % Replace with your actual time step
real_time = time * dt; % Convert indices to seconds

% Create figure with two subplots
figure;

% Plot Pitch vs Time with discrete points
subplot(2,1,1);
hold on;
plot(real_time, pitch, 'b.', 'MarkerSize', 8); % Blue dots
% Add target angle line
plot(real_time([1 end]), [-32 -32], 'r--', 'LineWidth', 1); % Red dashed line
hold off;
grid on;
title('Pitch vs Time');
xlabel('Time index');
ylabel('Pitch (degrees)');
legend('Measured Pitch', 'Target Pitch');

% Plot PWM vs Time with both points and interpolated line
subplot(2,1,2);
hold on;
plot(real_time, pwm, 'r.', 'MarkerSize', 8); % Red dots
plot(real_time, pwm, 'g-', 'LineWidth', 1); % Green interpolated line
hold off;
grid on;
title('PWM vs Time');
xlabel('Time index');
ylabel('PWM Value');

% Adjust layout
sgtitle('Drone Arm Motion Data'); % Overall title