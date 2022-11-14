data_str = readtable('experiment_output_categorized.csv');
data = readmatrix('experiment_output_categorized.csv');

font_size = 16
legend_pos = 'NorthWest'

% 1 = Key Generation
% 2 = Signing
% 3 = Verifying
%plotNum = 3

%figure('Position', [100 100 1000 600])
figure('Position', [100 100 1850 650])

algorithms = table2array(data_str(1:8, 1))
experiment = table2array(data_str(:, 2))
for plotNum=1:3
    subplot(1, 3, plotNum);
    hold on

    if plotNum == 1
        T = data(1:8, 4:6) % Columns 4 through 6
    elseif plotNum == 2
        T = data(9:16, 4:6) % Columns 4 through 6
    elseif plotNum == 3
        T = data(17:24, 4:6) % Columns 4 through 6
    end

    b = bar(T, 'stacked', 'FaceColor', 'flat')
    b(1).CData = hex2rgb('#F2668B');
    b(2).CData = hex2rgb('#3CA6A6');
    b(3).CData = hex2rgb('#253659');
    axis square
    ylabel('Assembly Instruction Count')

    % if plotNum == 1
    %     ylim([0, sum(max(T)) + 1.8e6])
    %     text_step_size = 2e5
    % elseif plotNum == 2
    %     ylim([0, sum(max(T)) + 2.9e6])
    %     text_step_size = 3e5
    % elseif plotNum == 3
    %     ylim([0, sum(max(T)) + 1.6e6])
    %     text_step_size = 1.8e5
    % end
    ylim([0, 7e6])
    text_step_size = 3e5
    text_font_size = 10
    for i=1:length(T)
        y = sum(T(i, :))
    %     text(i, y + 2 * text_step_size, addComma(T(i, 3)), 'FontSize' ,text_font_size, 'Color', 'black', 'HorizontalAlignment', 'center', 'VerticalAlignment', 'bottom');
    %     text(i, y + 1 * text_step_size, addComma(T(i, 2)), 'FontSize' ,text_font_size, 'Color', 'black', 'HorizontalAlignment', 'center', 'VerticalAlignment', 'bottom');
    %     text(i, y + 0 * text_step_size, addComma(T(i, 1)), 'FontSize' ,text_font_size, 'Color', 'black', 'HorizontalAlignment', 'center', 'VerticalAlignment', 'bottom');
        text(i, y + 2 * text_step_size, num2str(T(i, 3)), 'FontSize' ,text_font_size, 'Color', 'black', 'HorizontalAlignment', 'center', 'VerticalAlignment', 'bottom');
        text(i, y + 1 * text_step_size, num2str(T(i, 2)), 'FontSize' ,text_font_size, 'Color', 'black', 'HorizontalAlignment', 'center', 'VerticalAlignment', 'bottom');
        text(i, y + 0 * text_step_size, num2str(T(i, 1)), 'FontSize' ,text_font_size, 'Color', 'black', 'HorizontalAlignment', 'center', 'VerticalAlignment', 'bottom');
    end
    
    xticks(1:length(T))
    xticklabels(algorithms)
    xtickangle(40);

    xlim([1 - 0.75,length(T) + 0.75])


    if plotNum == 1
        title('Key Generation')
    elseif plotNum == 2
        title('Signing')
    elseif plotNum == 3
        title('Verifying')
    end
    set(gca,'FontSize', font_size);
    set(gca, 'XMinorTick','off', 'XMinorGrid','off');
    set(gca, 'YMinorTick','on', 'YMinorGrid','on');

    if plotNum == 1
        legend('Data Movement Instructions', 'Arithmetic and Logic Instructions', 'Control Flow Instructions', 'Location', legend_pos);
    end
end

function numOut = addComma(numIn)
   jf=java.text.DecimalFormat; % comma for thousands, three decimal places
   numOut= char(jf.format(numIn)); % omit "char" if you want a string out
end