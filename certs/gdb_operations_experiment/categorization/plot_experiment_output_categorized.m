format long

table_range = 'A1:M16'
data_str = readtable('experiment_output_categorized.csv','range',table_range);
data = readmatrix('experiment_output_categorized.csv','range',table_range);

font_size = 16
legend_pos = 'NorthWest'

% 1 = Key Generation
% 2 = Signing
% 3 = Verifying
plotNum = 3


if plotNum == 1
    % Keygen: Rows, Columns
    T = data(2:6, 4:6) % Columns 4 through 6
elseif plotNum == 2
    % Signing: Rows, Columns
    T = data(7:11, 4:6) % Columns 4 through 6
elseif plotNum == 3
    % Verifying: Rows, Columns
    T = data(12:16, 4:6) % Columns 4 through 6
end

figure('Position', [100 100 1000 620])
b = bar(T, 'stacked', 'FaceColor', 'flat')
b(1).CData = hex2rgb('#F2668B');
b(2).CData = hex2rgb('#3CA6A6');
b(3).CData = hex2rgb('#253659');
axis square
ylabel('Assembly Instruction Count')

if plotNum == 1
    ylim([0, sum(max(T)) + 1.7e6])
    text_step_size = 1.8e5
    xlabel('Key Generation')
elseif plotNum == 2
    ylim([0, sum(max(T)) + 2.8e6])
    text_step_size = 7e5
    xlabel('Message Sign')
elseif plotNum == 3
    ylim([0, sum(max(T)) + 1.5e6])
    text_step_size = 1.8e5
    xlabel('Message Verify')
end

text_font_size = 11
for i=1:length(T)
    y = sum(T(i, :))
    text(i, y + 2 * text_step_size, num2str(T(i, 3)), 'FontSize' ,text_font_size, 'Color', 'black', 'HorizontalAlignment', 'center', 'VerticalAlignment', 'bottom');
    text(i, y + 1 * text_step_size, num2str(T(i, 2)), 'FontSize' ,text_font_size, 'Color', 'black', 'HorizontalAlignment', 'center', 'VerticalAlignment', 'bottom');
    text(i, y + 0 * text_step_size, num2str(T(i, 1)), 'FontSize' ,text_font_size, 'Color', 'black', 'HorizontalAlignment', 'center', 'VerticalAlignment', 'bottom');
end

algorithms = table2array(data_str(:, 1))
experiment = table2array(data_str(:, 2))
xticklabels(algorithms)
xtickangle(40);

xlim([1 - 0.75,length(T) + 0.75])


set(gca,'FontSize', font_size);
set(gca, 'XMinorTick','off', 'XMinorGrid','off');
set(gca, 'YMinorTick','on', 'YMinorGrid','on');

legend('Data Movement Instructions', 'Arithmetic and Logic Instructions', 'Control Flow Instructions', 'Location', legend_pos);

function numOut = addComma(numIn)
   jf=java.text.DecimalFormat; % comma for thousands, three decimal places
   numOut= char(jf.format(numIn)); % omit "char" if you want a string out
end