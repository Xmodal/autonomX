//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#pragma once
#include <QObject>
#include <QList>
#include <QHash>
#include <QSharedPointer>
#include <QThread>
#include <vector>

#include "Generator.h"
#include "GOLPatternType.h"

class GameOfLife : public Generator
{
    Q_OBJECT
    Q_PROPERTY(int rule READ getRule WRITE writeRule NOTIFY ruleChanged)
    Q_PROPERTY(GOLPatternType GOLPattern READ getGOLPattern WRITE writeGOLPattern NOTIFY GOLPatternChanged)

private:

    // Debugging
    bool  flagDebug = false;

    // computation variables here etc etc
    std::vector<double> cells;
    std::vector<double> temp_cells;

    // properties and rules
    int rule = 102;

    //pattern name
     GOLPatternType GOLPattern = GOLPatternType::Random;

    // global iteration counter
    int iterationNumber;

    // GOL specific variables
    int currentGeneration;
    int lastGeneration;
    bool gameOver = false;


public:
     GameOfLife(int id, GeneratorMeta * meta);
    ~GameOfLife();

     void drawPattern(QString GOLPatternName);

    // overrides
    void computeIteration(double deltaTime) override;
    void initialize() override;
    //void resetParameters() override;
    double getLatticeValue(int x, int y) override;
    void writeLatticeValue(int x, int y, double value) override;

    // prop hooks
    int getRule();
    void writeRule(int rule);
    GOLPatternType getGOLPattern() const;
    void writeGOLPattern(GOLPatternType GOLPattern);

signals:
    // QML signals
    void ruleChanged(int rule);
    void GOLPatternChanged(GOLPatternType GOLPattern);
};
