//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#pragma once
#include <vector>

#include "Generator.h"

class GameOfLife : public Generator
{
    Q_OBJECT
    Q_PROPERTY(int rule READ getRule WRITE writeRule NOTIFY ruleChanged)
private:

    // Debugging
    bool  flagDebug = false;

    // computation variables here etc etc
    std::vector<double> cells;

    // properties and rules
    int rule = 102;

    // global iteration counter
    int iterationNumber;

    // GOL specific variables
    int currentGeneration;
    int lastGeneration;
    bool gameOver = false;


public:
     GameOfLife(int id, GeneratorMeta * meta);
    ~GameOfLife();

    // overrides
    void computeIteration(double deltaTime) override;
    void initialize() override;
    //void resetParameters() override;
    double getLatticeValue(int x, int y) override;
    void writeLatticeValue(int x, int y, double value) override;

    // prop hooks
    int getRule();
    void writeRule(int rule);

signals:
    // QML signals
    void ruleChanged(int rule);
};
